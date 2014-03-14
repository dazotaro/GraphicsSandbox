I made first:

1- cp BumpHeight.tga BumpHeight00.tga
   ...

2- montage -geometry 0 -tile 4x4 Bump_Height*.tga normal.tga

3- cp normal.tga normal00.tga
   ...

4- montage -geometry +0+0 -tile 2x2 normal??.tga normal_map.tga
