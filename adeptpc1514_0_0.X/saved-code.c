char EXIT_FLAG;
        
    /* Clear all rows except icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    EPD_PowerOff();
    menu_item = 1;
    EXIT_FLAG = 0;
    BUTTON_PRESS_OK = 0;
    
    while(!EXIT_FLAG){
        
        switch(menu_item){
            case 1:
                /* Display "Product Name" in 3rd row
                 * and the name in 4th row (to be defined). */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prod_name_str, 13);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
              
            case 2:    
                /* Display "Firmware Version" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, fw_version_str, 12);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
                
            case 3:    
                /* Display "Run h:min" in 3rd row and
                 * numbers in 4th row. */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, run_hr_min_str, 9);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;    
                
            case 4:    
                /* Display "On h:min" in 3rd row and
                 * numbers in 4th row. */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, on_hr_min_str, 8);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;   
                
            case 5:    
                /* Display "Interruptions" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, intrupts_str, 14);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;     
                
            case 6:    
                    /* Display "Exit" in 3rd row */
                    EPD_Init();
                    ClearDisplayRow(3);
                    SetUpDisplayString(ARROW_NO, prog_exit_str, 5);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_PowerOff();
                    break;    
        }
        
        /* Wait for key press */
        while(!BUTTON_PRESS_OK);    // only way to get out is with key press!
        
        BUTTON_PRESS_OK = 0;
        CheckUpShiftButtons(6);
        
        if(ENTER_BUTTON_PRESSED){
            ENTER_BUTTON_PRESSED = 0;
            EXIT_FLAG = 1;
        }
    }    
