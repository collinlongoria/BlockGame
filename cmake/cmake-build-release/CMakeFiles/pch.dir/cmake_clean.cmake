file(REMOVE_RECURSE
  "libpch.a"
  "libpch.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/pch.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
