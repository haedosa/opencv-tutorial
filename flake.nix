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
          pkgs.my-opencv
          pkgs.cudatoolkit
          pkgs.cmake
          pkgs.clang-tools_12
          pkgs.python3
        ];
      };
    };

    packages.${system} = {
      default = pkgs.opencv-tutorial;
      my-opencv = pkgs.my-opencv;
    };

    overlay = final: prev:
    {
      opencv-tutorial = final.callPackage ./. {};
      my-opencv = ((final.callPackage ./opencv/opencv4.nix {
        inherit (final.darwin.apple_sdk.frameworks)
          AVFoundation Cocoa VideoDecodeAcceleration CoreMedia MediaToolbox Accelerate;
        pythonPackages = final.python3Packages;
      }).overrideAttrs (oldAttrs: {
        cmakeFlags = oldAttrs.cmakeFlags ++ [
          "-DWITH_OPENMP=OFF"
          "-DCPU_BASELINE=AVX"
          "-DCPU_DISPATCH=AVX2"
        ];
      })).override (old:
        {
          enableGtk2 = true;
          enableCuda = true;
          enableCudnn = false;
          enableIpp = false;
          enablePython = false;
          runAccuracyTests = true;
          runPerformanceTests = true;
        }
      );
    };
  };
}
