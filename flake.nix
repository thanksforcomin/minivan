{
  description = "a nix flake for the stuff";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config = {};
          overlays = [];
        };
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            glfw
            libGL
            libglvnd
            mesa
          ];

          packages = with pkgs; [
            # Vulkan
            vulkan-tools
            vulkan-headers
            vulkan-validation-layers
            vulkan-loader
            vulkan-utility-libraries

            # GLFW
            glfw3
            glfw

            # GLM (math)
            glm

            # OpenGL
            libGL
            libGLU
            libglvnd
            glew
            freeglut
            mesa
            xorg.libX11
            xorg.libXext
            xorg.libXrandr
            xorg.libXcursor
            xorg.libXi

            # Build tools
            cmake
            pkg-config
            # meson  # uncomment if you switch
          ];

          # Help CMake find Vulkan
          Vulkan_LIBRARY = "${pkgs.vulkan-loader}/lib/libvulkan.so";
          Vulkan_INCLUDE_DIR = "${pkgs.vulkan-headers}/include";
          
          # Optional: Add Vulkan to LD_LIBRARY_PATH for runtime
          shellHook = ''
            export LD_LIBRARY_PATH=${pkgs.vulkan-loader}/lib:$LD_LIBRARY_PATH
          '';
        };
      });
}
