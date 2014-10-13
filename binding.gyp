{
  'targets': [
    {
      'target_name': 'gamepad',
      'sources': [
        'binding.cc',
        'gamepad/source/gamepad/Gamepad_macosx.c',
        'gamepad/source/gamepad/Gamepad_private.c',
      ],
      'include_dirs': [
        './gamepad/source',
      ],
    }
  ]
}
