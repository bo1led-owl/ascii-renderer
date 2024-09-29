project('ascii_renderer', 'c', default_options : [
  'c_std=c99',
  'warning_level=3',
])

incdir = include_directories('dep')
cc = meson.get_compiler('c')
m_dep = cc.find_library('m')

add_project_arguments('-DPROJECT_NAME=' + meson.project_name(), language: 'c')

src = [
   'src/main.c',
   'src/cli.c',
   'src/render.c',
   'dep/stb_image.c',
]
executable(
  'ascii',
  src,
  include_directories: [incdir],
  dependencies: [m_dep],
)
