project('dotris', 'c',
  version : '1.0.0',
  default_options : ['warning_level=3', 'buildtype=debugoptimized'])

if get_option('buildtype') == 'debugoptimized'
  add_global_arguments('-DDEBUG', language : 'c')
endif

src = ['src/main.c', 'src/tetriminos.c', 'src/grid.c']
ncurses = dependency('ncursesw')
inc = include_directories('./include')
executable('dotris',
           src,
           dependencies : ncurses,
           include_directories : inc,
           install : true)
