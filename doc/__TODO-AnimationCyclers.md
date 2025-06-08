


## Cityscape_Window()

    /*
        How to systematize and make configurable, these animation cycles?
    */
    // cityscape_bldg_anim_ctr  = ((cityscape_bldg_anim_ctr  + 1) %  9);
    // cityscape_water_anim_ctr = ((cityscape_water_anim_ctr + 1) % 12);
    // cityscape_wall_anim_ctr  = ((cityscape_wall_anim_ctr  + 1) %  4);

    cityscape_bldg_anim_itr++;
    if(cityscape_bldg_anim_itr == 60)  // IDK ... (9 * F) + (F - 1) + 1 ... 
    {
        cityscape_bldg_anim_itr = 6;   // IDK ... F/F = 1
    }
    cityscape_bldg_anim_ctr = (cityscape_bldg_anim_itr / 6);  // { 1, 1, 1, 1, ..., 9, 9, 9, 9 }

    cityscape_water_anim_itr++;
    if(cityscape_water_anim_itr == 84)
    {
        cityscape_water_anim_itr = 6;
    }
    cityscape_water_anim_ctr = (cityscape_water_anim_itr / 6);

    cityscape_wall_anim_itr++;
    if(cityscape_wall_anim_itr == 60)
    {
        cityscape_wall_anim_itr = 6;
    }
    cityscape_wall_anim_ctr = (cityscape_wall_anim_itr / 6);





## cityscape_wall_anim_ctr
    cityscape_wall_anim_ctr  = ((cityscape_wall_anim_ctr  + 1) %  4);
    0 + 1 % 4 = 1
    1 + 1 % 4 = 2
    2 + 1 % 4 = 3
    3 + 1 % 4 = 4
    4 + 1 % 4 = 1
    ...so on and so forth...
presumably, updates per screen draw...
...and I want a configurable formula for
...not {1,2,3,4}, but {1,1,2,2,3,3,4,4} or {1,1,1,2,2,2,3,3,3,4,4,4} or some X slowdown
...based on frame-rate?


    cityscape_bldg_anim_ctr  = ((cityscape_bldg_anim_ctr  + 1) %  9);
    cityscape_water_anim_ctr = ((cityscape_water_anim_ctr + 1) % 12);
    cityscape_wall_anim_ctr  = ((cityscape_wall_anim_ctr  + 1) %  4);

    Line((xstart      ), (ystart     ), (xstart + 204), (ystart     ), ST_TRANSPARENT);
    Line((xstart      ), (ystart + 95), (xstart + 204), (ystart + 95), ST_TRANSPARENT);
    Line((xstart      ), (ystart     ), (xstart      ), (ystart + 95), ST_TRANSPARENT);
    Line((xstart + 204), (ystart     ), (xstart + 204), (ystart + 95), ST_TRANSPARENT);

    if(cityscape_build_anim_ctr < 100)
    {
        cityscape_build_anim_ctr += 10;
    }


Outpost_Cityscape_Draw() uses these for the backgrounds
