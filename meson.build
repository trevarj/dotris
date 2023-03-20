project('dotris', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'buildtype=debugoptimized'])

if get_option('buildtype') == 'debugoptimized'
  add_global_arguments('-DDEBUG', language : 'c')
endif

src = ['main.c', 'tetriminos.c', 'grid.c']
ncurses = dependency('ncursesw')
executable('dotris',
           src,
           dependencies : ncurses,
           install : false)

