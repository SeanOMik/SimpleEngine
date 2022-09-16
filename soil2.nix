{ lib
, stdenv
, fetchFromGitHub
, cmake
, libGL
, libX11
}:

stdenv.mkDerivation rec {
  version = "39028e64921c03cabbc53b937da4a85aba264e00";
  pname = "soil2";

  src = fetchFromGitHub {
    owner = "SpartanJ";
    repo = pname;
    rev = version;
    sha256 = "sha256-zQQ8lwOkMCxdlf6zfnIOYVUTGVqnJuHL/LL8fbzxwHY=";
  };

  nativeBuildInputs = [ cmake libGL libX11 ];

  installPhase = ''
    runHook preInstall

    cmake --build . --target install --config Release

    runHook postInstall
  '';

  meta = with lib; {
    description = "SOIL2 is a tiny C library used primarily for uploading textures into OpenGL.";
    homepage = "https://github.com/SpartanJ/SOIL2";
    license = licenses.mit0;
    platforms = platforms.unix;
    maintainers = with maintainers; [ seanomik ];
  };
}