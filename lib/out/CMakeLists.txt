add_library(out
	custom.c
	gui_guider.c
	events_init.c
	setup_scr_screen.c
	widgets_init.c
	lv_font_montserratMedium_16.c
	lv_font_montserratMedium_32.c
)

target_include_directories(out PUBLIC
	.
)

target_link_libraries(out pico_stdlib hardware_spi hardware_i2c hardware_dma hardware_pwm hardware_adc lvgl)
