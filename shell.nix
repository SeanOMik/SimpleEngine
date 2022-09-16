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
    (callPackage ./soil2.nix { })
  ];
}