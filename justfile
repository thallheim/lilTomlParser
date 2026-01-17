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


alias ta := test-all-debug
[group("Unit tests"), doc("Run all unit tests")]
@test-all-debug:
    ctest --test-dir build/ --output-on-failure -C "Debug"

alias tar := test-all-release
[group("Unit tests"), doc("Run all unit tests")]
@test-all-release:
    ctest --test-dir build/ --output-on-failure -C "Release"

alias lt := list-tests
[group("Unit tests"), doc("List available tests")]
@list-tests:
    ctest --test-dir build/ -N

alias t := test
[group("Unit tests"), doc("Run specific unit test (Debug conf.)")]
@test ARG:
    ctest --test-dir build/ --output-on-failure -C "Debug" -R {{ARG}}

alias tr := test-release
[group("Unit tests"), doc("Run specific unit test (Release conf.)")]
@test ARG:
    ctest --test-dir build/ --output-on-failure -C "Release" -R {{ARG}}
