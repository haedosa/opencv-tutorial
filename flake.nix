{
  inputs = {
    haedosa.url = "github:haedosa/flakes";
    nixpkgs.follows = "haedosa/nixpkgs";
  };

  outputs = { self, nixpkgs, ... }@inputs:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; overlays = [self.overlay]; config = { allowUnfree = true; }; };
  in rec {
    devShells.${system} = {
      default = pkgs.mkShell {
        buildInputs = [
          pkgs.opencv-custom
          pkgs.cudatoolkit
          pkgs.cmake
          pkgs.clang-tools_12
        ];
      };
    };

    packages.${system} = {
      default = pkgs.opencv-tutorial;
      opencv-custom = pkgs.opencv-custom;
    };

    overlay = final: prev:
    {
      opencv-tutorial = final.callPackage ./. {};
      opencv-custom = prev.opencv4.overrideAttrs (oldAttrs: {
        cmakeFlags = oldAttrs.cmakeFlags ++ [
          "-DWITH_OPENMP=OFF"
        ];
      });
    };
  };
}
