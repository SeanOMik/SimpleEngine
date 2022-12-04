{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  # nativeBuildInputs is usually what you want -- tools you need to run
  nativeBuildInputs = with pkgs; [
    gdb
    lldb
    cmake
    pkg-config
    ninja
    glew
    glfw
    glm
    assimp
    spdlog
    bullet
    rttr
    lua5_4
    (callPackage ./sol2.nix { })
  ];
}