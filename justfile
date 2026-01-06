@_default:
    just --list --unsorted

alias l := _pick
[no-exit-message]
@_pick:
    just --choose


alias b := build
[group("build"), doc("Build - debug config")]
@build *ARGS:
    cmake --build build --config "Debug" {{ARGS}}

alias br := build-release
[group("build"), doc("Build - release config")]
@build-release *ARGS:
    cmake --build build --config "Release" {{ARGS}}

alias bc := build-clang
[working-directory: 'build']
[group("build")]
@build-clang:
    make



alias conf := configure
[group("configure build"), doc("Configure for Ninja")]
@configure:
    cmake -B build -S . -G "Ninja Multi-Config"

alias reconf := reconfigure
[group("configure build"), doc("Reconfigure for Ninja (--fresh)")]
@reconfigure:
    cmake -B build -S . -G "Ninja Multi-Config" --fresh


alias conf-make := configure-make
[group("configure build"), doc("Configure for Make")]
@configure-make:
    cmake -B build -S .

alias reconf-make := reconfigure-make
[group("configure build"), doc("Reconfigure for Make (--fresh)")]
@reconfigure-make:
    cmake -B build -S . --fresh
