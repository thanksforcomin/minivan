project('vulkan-gltf-app', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=1'
  ]
)

vulkan_dep = dependency('vulkan', version: '>=1.3.238', required: true)
sdl2_dep = dependency('sdl2')
glm_dep = dependency('glm')

cmake = import('cmake')

vk_bootstrap_opts = cmake.subproject_options()
vk_bootstrap_opts.add_cmake_defines({'CMAKE_POSITION_INDEPENDENT_CODE': 'ON'})

vk_bootstrap_sub_proj = cmake.subproject('vk-bootstrap', options: vk_bootstrap_opts)
vk_bootstrap_dep = vk_bootstrap_sub_proj.dependency('vk-bootstrap')

vk_mem_alloc_sub_proj = cmake.subproject('VulkanMemoryAllocator')
vk_mem_alloc_dep = vk_mem_alloc_sub_proj.dependency('VulkanMemoryAllocator')

sources = files(
  'src/main.cpp',
  'src/vulkan_context.cpp'
)

executable('small',
  sources,
  dependencies: [
    vulkan_dep,
    sdl2_dep,
    glm_dep,
    vk_bootstrap_dep,
    vk_mem_alloc_dep
  ],
  install: false,
  override_options: ['cpp_std=c++23']
)