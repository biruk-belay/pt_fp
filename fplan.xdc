# User Generated miscellaneous constraints


set_property HD.RECONFIGURABLE true [get_cells freq_div]
create_pblock pblock_freq_div
add_cells_to_pblock [get_pblocks pblock_freq_div] [get_cells -quiet [list freq_div]]
resize_pblock [get_pblocks pblock_freq_div] -add {SLICE_X0Y0:SLICE_X705Y18446744073709551615}
resize_pblock [get_pblocks pblock_freq_div] -add {RAMB18_X0Y0:RAMB18_X18446744069448138752Y18446744073709551615}
resize_pblock [get_pblocks pblock_freq_div] -add {RAMB36_X0Y0:RAMB36_X18446744073709551615Y18446744073709551615}
resize_pblock [get_pblocks pblock_freq_div] -add {DSP48_X0Y0:DSP48_X35926304Y18446744073709551615}
set_property RESET_AFTER_RECONFIG true [get_pblocks pblock_freq_div]
set_property SNAPPING_MODE ON [get_pblocks pblock_freq_div]


