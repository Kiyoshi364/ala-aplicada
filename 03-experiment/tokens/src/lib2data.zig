const std = @import("std");
const os = std.os;
const fs = std.fs;

const Tokenizer = std.zig.Tokenizer;
const Token = std.zig.Token;
const Tag = Token.Tag;

pub fn main() !void {

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const malloc = gpa.allocator();
    defer _ = gpa.deinit();

    const base_dir = "zig_std/";

    const cwd = std.fs.cwd();
    var itdir = cwd.openIterableDir(base_dir, .{
        .access_sub_paths = true,
    }) catch |err| {
        std.debug.print("unhandled error: {}", .{err});
        os.exit(1);
    };

    var arr_list = try std.ArrayList([]const u8).initCapacity(malloc, tag_len);

    const state = State{
        .matrix = try cwd.createFile("matrix.txt", .{}),
        .docs = try cwd.createFile("docs.txt", .{}),
        .tokens = try cwd.createFile("tokens.txt", .{}),
        .id_list = &arr_list,
    };
    defer state.deinit();
    try traverse(itdir, state, malloc);
}

const State = struct {
    matrix: fs.File,
    docs: fs.File,
    tokens: fs.File,
    id_list: *std.ArrayList([]const u8),

    fn deinit(self: State) void {
        self.matrix.close();
        self.docs.close();
        self.tokens.close();
        for (self.id_list.items) |id| {
            self.id_list.allocator.free(id);
        }
        self.id_list.deinit();
    }

    fn add_doc(self: State, doc_path: []const u8) !void {
        try self.docs.writer().print("{s}\n", .{doc_path});
    }

    fn add_tokens(self: State) !void {
        inline for (@typeInfo(Tag).Enum.fields) |field| {
            if (tag2id_table[field.value] != null) {
                try self.tokens.writer().print("tk_{s}\n", .{field.name});
            }
        }
    }

    fn add_token_id(self: State, id: []const u8) !void {
        try self.tokens.writeAll(id);
        try self.tokens.writeAll("\n");
    }

    fn add_counts_to_matrix(self: State, tk_count: std.ArrayList(usize)) !void {
        const last_i = blk: {
            // Find last non-zero value
            var i = tk_count.items.len;
            break :blk while (i > 0) : (i -= 1) {
                if (tk_count.items[i-1] != 0) {
                    break i;
                }
            } else 1;
        };
        var i = @as(usize, 0);
        while (i < last_i) : (i += 1) {
            try self.matrix.writer().print("{s}{d}", .{
                if (i > 0) " " else "",
                tk_count.items[i],
            });
        }
        try self.matrix.writeAll("\n");
    }

    fn find_id(self: State, id: []const u8) ?usize {
        return for (self.id_list.items) |s, i| {
            if (std.mem.eql(u8, id, s)) {
                break i;
            }
        } else null;
    }

    fn add_id(self: State, id: []const u8) !usize {
        std.debug.assert(self.find_id(id) == null);
        const new_id = try self.id_list.allocator.alloc(u8, id.len);
        std.mem.copy(u8, new_id, id);
        try self.id_list.append(new_id);
        try self.add_token_id(id);
        return self.id_list.items.len - 1;
    }
};

fn traverse(
    itdir: fs.IterableDir,
    state: State,
    alloc: std.mem.Allocator,
) anyerror!void {
    std.debug.print("Traversing dir\n", .{});

    try state.add_tokens();

    var tk_count = try std.ArrayList(usize).initCapacity(
        alloc,
        2*tag_len,
    );
    defer tk_count.deinit();
    tk_count.appendNTimesAssumeCapacity(0, tag_len);

    var walker = try itdir.walk(alloc);
    defer walker.deinit();
    while (try walker.next()) |entry| {
        if (entry.kind != .File) {
            continue;
        }
        if (!std.mem.eql(
            u8,
            ".zig",
            entry.basename[entry.basename.len-4..],
        )) {
            std.debug.print("Skipping path: '{s}'\n", .{
                entry.path,
            });
            continue;
        }
        std.debug.print("Walking path: '{s}'\n", .{
            entry.path,
        });

        const file = entry.dir.openFile(entry.basename, .{}) catch |err|
            {
            std.debug.print(
                "Couldn't open file {s}: {}",
                .{ entry.path, err },
            );
            continue;
        };
        defer file.close();

        const buffer = try file.readToEndAllocOptions(
            alloc,
            0xFF_FF_FF_FF,
            null,
            @alignOf(u8),
            @as(u8, 0),
        );
        defer alloc.free(buffer);

        try state.add_doc(entry.path);
        try tokenize(buffer, state, &tk_count);

        try state.add_counts_to_matrix(tk_count);

        const old_len = tk_count.items.len;
        tk_count.clearRetainingCapacity();
        tk_count.appendNTimesAssumeCapacity(0, old_len);
    }
}

fn tokenize(buffer: [:0]const u8, state: State, tk_count: *std.ArrayList(usize)) !void {
    var tokenizer = Tokenizer.init(buffer);
    var tk = tokenizer.next();
    while (tk.tag != .eof) : (tk = tokenizer.next()) {
        if (token_tag_to_id(tk.tag)) |index| {
            tk_count.items[index] += 1;
        } else if (tk.tag == .identifier) {
            const id = buffer[tk.loc.start..tk.loc.end];
            if (state.find_id(id)) |index| {
                tk_count.items[index] += 1;
            } else {
                const id_i = try state.add_id(id);
                std.debug.assert(tk_count.items.len == tag_len + id_i);
                try tk_count.append(1);
            }
        } else {
            const should_ignore = for (tags_to_ignore) |ig_tag| {
                if (ig_tag == tk.tag) {
                    break true;
                }
            } else false;
            std.debug.assert(should_ignore);
        }
    }
}

const tags_to_ignore = [_]Tag{
    .invalid,
    .invalid_periodasterisks,
    .identifier,
    .string_literal,
    .multiline_string_literal_line,
    .char_literal,
    .eof,
    .number_literal,
    .doc_comment,
    .container_doc_comment,
};
const tag_len = @typeInfo(Tag).Enum.fields.len;
const tag2id_table: [tag_len]?usize = tag2id_tuple.table;
const id_start: usize = tag2id_tuple.count;
const tag2id_tuple = blk: {
    var tag2id_table_ = @as([tag_len]?usize, undefined);
    var curr_val = @as(usize, 0);
    @setEvalBranchQuota(3_000);
    for (@typeInfo(Tag).Enum.fields) |field, i| {
        const should_ignore = for (tags_to_ignore) |ig_tag| {
            if (std.mem.eql(u8, @tagName(ig_tag), field.name)) {
                break true;
            }
        } else false;
        if (should_ignore) {
            tag2id_table_[i] = null;
        } else {
            tag2id_table_[i] = curr_val;
            curr_val += 1;
        }
    }
    break :blk .{
        .table = tag2id_table_,
        .count = curr_val,
    };
};
fn token_tag_to_id(tag: Tag) ?usize {
    return tag2id_table[@enumToInt(tag)];
}
