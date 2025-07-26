


vs. _UNITS[].Draw_Priority and entities_on_movement_map[entity_table_idx], Set_Entities_On_Map_Window()?
    only decides which STATFIG gets drawn for the stack, by way of setting its idx in the entities_on_movement_map array



combat_grid_entities[]
combat_grid_entity_count

entt->draw_order_value = ((cgy * 8000) + (cgx * 320) + (cgy_ofst * 16) + cgx_ofst)
combat_grid_entities_draw_order[]



CMB_EntitiesReset()
    combat_grid_entity_count = 0;
    for(itr = 0; itr < MAX_ENTITIES; itr++)
        combat_grid_entities_draw_order[itr] = itr;  // ; from farthest to nearest combat entities


CMB_CreateEntity__WIP()

    entt->draw_x = draw_x;
    entt->draw_y = draw_y;
    entt->seg_or_idx = seg_or_idx;
    entt->frame_num = frame_num;
    entt->draw_x_shift = draw_x_shift;
    entt->draw_y_shift = draw_y_shift;
    entt->entity_type = entity_type;
    entt->owner_idx = owner_idx;
    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &cgx, &cgx_ofst);
    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &cgy, &cgy_ofst);
    entt->draw_order_value = ((cgy * 8000) + (cgx * 320) + (cgy_ofst * 16) + cgx_ofst);
