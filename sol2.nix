{ lib
, stdenv
, fetchFromGitHub
, cmake
, clang
, llvm
, pkgs
}:

let
  stdenv = pkgs.clangStdenv;
in stdenv.mkDerivation rec {
  version = "v3.3.0";
  pname = "sol2";

  src = fetchFromGitHub {
    owner = "ThePhD";
    repo = pname;
    rev = version;
    sha256 = "sha256-NACIWXy1GHuYNf7e3Go+S4PSQqitzgErOvqkmXQVuLw=";
  };

  nativeBuildInputs = [ cmake clang pkgs.lua5_4 ];

  depsBuildBuild = [ pkgs.lua5_4 ];

#  stdenv = pkgs.clangStdenv;
  cmakeFlags = [ "-DSOL2_BUILD_LUA=FALSE" "-DSOL2_LUA_VERSION='5.4.4'" ];

  installPhase = ''
    runHook preInstall

    cmake --build . --target install --config Release

    runHook postInstall
  '';

  meta = with lib; {
    description = "sol2 is a C++ library binding to Lua.";
    homepage = "https://github.com/ThePhD/sol2";
    license = licenses.mit;
    platforms = platforms.unix;
    maintainers = with maintainers; [ seanomik ];
  };
}
