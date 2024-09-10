


## ITEM_Draw_Seg

    ITEM_LoadCraftingRes+1D9    mov     [ITEM_Draw_Seg], ax             ; 30 sandbox paragraphs (480 bytes)                
    IDK_CreateArtifact_Draw+62D push    [ITEM_Draw_Seg]                 ; Draw_Seg                                         
    IDK_CreateArtifact_Draw+63C push    [ITEM_Draw_Seg]                 ; pict_seg                                         
    Merchant_Popup_Load+59      mov     [ITEM_Draw_Seg], ax             ; 30 sandbox paragraphs (480 bytes)                
    Merchant_Popup_Draw+17      push    [ITEM_Draw_Seg]                 ; Draw_Seg                                         
    Merchant_Popup_Draw+104     push    [ITEM_Draw_Seg]                 ; pict_seg                                         
    Hire_Hero_Load+61           mov     [ITEM_Draw_Seg], ax             ; should use the already allocated USW_ItemDraw_Seg
    Hire_Hero_Popup_Draw+13     push    [ITEM_Draw_Seg]                 ; should use USW_ItemDraw_Seg                      
    Hire_Merc_Popup_Draw+13     push    [ITEM_Draw_Seg]                 ; IDrawSeg                                         

...vs. item_view_popup_bitm



220 x 83 = 18,260 bytes

Item_Pool_View()
    item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200);  // 1200 PR, 19200 B
    m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 60);  // 60 PR, 960 B

Item_Pool_View_Draw()
    Item_View_Prepare(item_pool_item_idx, item_view_popup_bitm, m_item_icon_workarea);


Merchant_Popup_Load()
    ITEM_Draw_Seg = Allocate_Next_Block(_screen_seg, 1200);
    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 30);

Merchant_Popup_Draw()
    Item_View_Prepare(GUI_InHeroNaming, ITEM_Draw_Seg, IMG_SBK_PageText);
