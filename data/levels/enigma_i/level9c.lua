--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 58
levelh = 37

create_world(levelw, levelh)
draw_border( "st-greenbrown")
fill_floor("fl-abyss", 0,0, levelw,levelh)

oxyd_default_flavor = "d"

oxyd ( 7, 5)
oxyd ( 7,30)
oxyd (13,10)
oxyd (22,14)
oxyd (27,14)
oxyd (22,17)
oxyd (27,17)
oxyd (53,30)

oxyd_shuffle()

fill_floor("fl-metal",  6, 8,  3, 7)
fill_floor("fl-metal",  2,15, 16, 3)
fill_floor("fl-metal", 12, 7,  3, 1)
fill_floor("fl-metal",  2,18,  2,13)
fill_floor("fl-normal", 4,30,  1, 1)
fill_floor("fl-metal", 13,13,  1,2)
fill_floor("fl-metal", 17,16,  3,1)
fill_floor("fl-normal", 51, 2, 5,24)
fill_floor("fl-normal", 53,26, 1, 2)
fill_floor("fl-normal", 10,29, 8, 1)
fill_floor("fl-normal", 10,31, 8, 1)
fill_floor("fl-normal", 37, 2,14, 3)
fill_floor("fl-normal", 37, 5, 2, 2)
fill_floor("fl-metal", 38, 8, 11,19)
fill_floor("fl-normal", 43, 6, 1, 2)
fill_floor("fl-normal", 43,27, 1, 3)
fill_floor("fl-normal", 27,29, 6, 1)
fill_floor("fl-normal", 27,31, 6, 1)
fill_floor("fl-metal", 10,30, 41, 1)
fill_floor("fl-metal", 10, 4,  8, 3)
fill_floor("fl-metal", 27, 4, 10, 3)

set_stone( "st-actorimpulse",24,15)
set_stone( "st-actorimpulse",25,15)
set_stone( "st-actorimpulse",24,16)
set_stone( "st-actorimpulse",25,16)

draw_stones( "st-grate1", {33,30}, {1,0},17)
draw_stones( "st-grate1", {43,27}, {0,1}, 3)
draw_stones( "st-grate2", {18,16}, {1,0}, 1)

draw_stones( "st-rock6", {33,29}, {1,0},10)
draw_stones( "st-rock6", {44,29}, {1,0}, 7)
draw_stones( "st-rock6", {33,31}, {1,0},18)
draw_stones( "st-rock6", {12,13}, {0,1}, 2)
draw_stones( "st-rock6", {14,13}, {0,1}, 2)
draw_stones( "st-rock6", {38, 8}, {0,1},19)
draw_stones( "st-rock6", {48, 8}, {0,1},19)
draw_stones( "st-rock6", {40,10}, {0,1},15)
draw_stones( "st-rock6", {46,10}, {0,1},15)
draw_stones( "st-rock6", {42,12}, {0,1},11)
draw_stones( "st-rock6", {44,12}, {0,1},11)
draw_stones( "st-rock6", {42,26}, {0,1}, 3)
draw_stones( "st-rock6", {44,26}, {0,1}, 3)
draw_stones( "st-rock6", {39, 8}, {1,0}, 4)
draw_stones( "st-rock6", {44, 8}, {1,0}, 4)
draw_stones( "st-rock6", {39,26}, {1,0}, 3)
draw_stones( "st-rock6", {45,26}, {1,0}, 3)
draw_stones( "st-rock6", {41,10}, {1,0}, 2)
draw_stones( "st-rock6", {44,10}, {1,0}, 2)
draw_stones( "st-rock6", {41,24}, {1,0}, 2)
draw_stones( "st-rock6", {44,24}, {1,0}, 2)
draw_stones( "st-greenbrown_hole", {51, 8}, {0,1}, 13)
draw_stones( "st-greenbrown_hole", {52, 8}, {0,1}, 13)
draw_stones( "st-greenbrown_hole", {53, 8}, {0,1}, 13)
draw_stones( "st-greenbrown_hole", {54, 8}, {0,1}, 13)
draw_stones( "st-greenbrown_hole", {55, 8}, {0,1}, 13)

set_stone("st-oneway-e",10, 5)
set_stone("st-oneway-s",13,13)
set_stone("st-oneway-s",13,14)
set_stone("st-oneway-s",39,16)
set_stone("st-oneway-s",41,16)
set_stone("st-oneway-s",43,16)
set_stone("st-oneway-s",45,16)
set_stone("st-oneway-s",47,16)
set_stone("st-oneway-s",53,26)
set_stone("st-oneway-w",50,30)

function basis1( x, y)
       draw_floor("fl-gradient", {x-1,y}, {0,1}, 3, {type=3})
       draw_floor("fl-gradient", {x,y-1}, {1,0}, 3, {type=1})
       draw_floor("fl-gradient", {x+3,y}, {0,1}, 3, {type=4})
       set_floor("fl-gradient",   x-1, y-1,         {type=5})
       set_floor("fl-gradient",   x+3, y-1,         {type=6})
       set_floor("fl-gradient",   x-1, y+3,         {type=7})
       set_floor("fl-gradient",   x+3, y+3,         {type=8})
       draw_floor("fl-gradient", {x,y+3}, {1,0}, 3, {type=2})
       fill_floor("fl-normal",    x,y, 3, 3)
end

basis1 ( 6, 4)
basis1 ( 6,29)
basis1 (12, 9)
basis1 (52,29)

function basis2( x, y)
       draw_floor("fl-gradient", {x-1,y}, {0,1}, 6, {type=3})
       draw_floor("fl-gradient", {x,y-1}, {1,0}, 8, {type=1})
       draw_floor("fl-gradient", {x+8,y}, {0,1}, 6, {type=4})
       set_floor("fl-gradient",   x-1, y-1,         {type=5})
       set_floor("fl-gradient",   x+8, y-1,         {type=6})
       set_floor("fl-gradient",   x-1, y+6,         {type=7})
       set_floor("fl-gradient",   x+8, y+6,         {type=8})
       draw_floor("fl-gradient", {x,y+6}, {1,0}, 8, {type=2})
       fill_floor("fl-normal",    x,y, 8, 6)
end

basis2 (21,13)

function basis3( x, y)
       draw_floor("fl-gradient", {x-1,y}, {0,1}, 3, {type=4})
       draw_floor("fl-gradient", {x,y-1}, {1,0}, 7, {type=2})
       draw_floor("fl-gradient", {x+7,y}, {0,1}, 3, {type=3})
       set_floor("fl-gradient",   x-1, y-1,         {type=12})
       set_floor("fl-gradient",   x+7, y-1,         {type=10})
       set_floor("fl-gradient",   x-1, y+3,         {type=11})
       set_floor("fl-gradient",   x+7, y+3,         {type=9})
       draw_floor("fl-gradient", {x,y+3}, {1,0}, 7, {type=1})
       fill_floor("fl-ice_001",   x,y, 7, 3)
end

basis3 (19, 4)

set_item( "it-spring2", 37, 5)
set_item( "it-spring2", 43, 6)

document(43,26, " ")

set_actor("ac-blackball",55.5,32.5)















