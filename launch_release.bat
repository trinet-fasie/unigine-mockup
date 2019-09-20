@echo off
chcp 65001
setlocal EnableDelayedExpansion
set app=bin\unigine_project_x64.exe

start %app%  -video_app auto -video_vsync 0 -video_refresh 0 -video_mode 1 -video_resizable 1 -video_fullscreen 0 -video_debug 0 -video_gamma 1.000000 -sound_app auto -data_path "../data/"  -engine_config "../data/unigine.cfg" -extern_plugin "FbxImporter" -console_command "config_readonly 1 && world_load \"unigine_project\""
