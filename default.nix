{ stdenv, my-opencv, cmake, cudatoolkit, ... }:

stdenv.mkDerivation {
  name = "opencv-tutorial";

  src = ./.;

  buildInputs =
    [
      my-opencv
      cudatoolkit
      cmake
    ];

  cmakeFlags = [
  ];
}
