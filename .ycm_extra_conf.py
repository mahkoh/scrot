def FlagsForFile(filename):
  return {
    'flags': [ '-Wall',
               '-Wpedantic',
               '-Werror',
               '-std=c11',
             ],
    'do_cache': True
  }
