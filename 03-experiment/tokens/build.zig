const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    const lib2data = b.addExecutable("lib2data", "src/lib2data.zig");
    lib2data.setTarget(target);
    lib2data.setBuildMode(mode);
    lib2data.install();

    const run_lib2data = lib2data.run();
    run_lib2data.step.dependOn(&lib2data.step);

    const lib2data_step = b.step("lib2data", "Walk the stdlib and write .txt's with data");
    lib2data_step.dependOn(&run_lib2data.step);

    const run_step = b.step("run", "Run lib2data");
    run_step.dependOn(b.getInstallStep());
    run_step.dependOn(lib2data_step);
}
