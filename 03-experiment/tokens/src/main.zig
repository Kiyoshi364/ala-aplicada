const std = @import("std");
const os = std.os;
const fs = std.fs;

const Tokenizer = std.zig.Tokenizer;

pub fn main() !void {
    // const stdout_file = std.io.getStdOut().writer();
    // var bw = std.io.bufferedWriter(stdout_file);
    // const stdout = bw.writer();

    // try stdout.print("asdf", .{});
    // try bw.flush();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const malloc = gpa.allocator();

    const base_dir = "zig_std/";
    // const filename = "zig/tokenizer.zig";
    // const filename = "hash.zig";

    const cwd = std.fs.cwd();
    var itdir = cwd.openIterableDir(base_dir, .{
        .access_sub_paths = true,
    }) catch |err| {
        std.debug.print("unhandled error: {}", .{err});
        os.exit(1);
    };

    try traverse(itdir, malloc);
}

fn traverse(
    itdir: fs.IterableDir,
    alloc: std.mem.Allocator,
) anyerror!void {
    std.debug.print("Traversing dir\n", .{});

    var walker = try itdir.walk(alloc);
    defer walker.deinit();
    while (try walker.next()) |entry| {
        if (entry.kind != .File) {
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
        tokenize(buffer);
        break;
    }
}

fn tokenize(buffer: [:0]const u8) void {
    var tokenizer = Tokenizer.init(buffer);
    var tk = tokenizer.next();
    while (tk.tag != .eof) : (tk = tokenizer.next()) {
        tokenizer.dump(&tk);
    }
}
