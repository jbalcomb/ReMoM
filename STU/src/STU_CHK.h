#ifndef STU_CHK_H
#define STU_CHK_H



#ifdef __cplusplus
extern "C" {
#endif


void Capture_City_Count(void);
// static void Check_City_Count(void);
void Capture_Player_Count(void);
// static void Check_Player_Count(void);
void Capture_Unit_Count(void);
// static void Check_Unit_Count(void);

void Capture_World_Map_Data(void);
// static void Check_World_Map_Data(void);
void Release_World_Map_Data(void);

void Capture_Cities_Data(void);
// static void Check_Cities_Data(void);
void Release_Cities_Data(void);

void Capture_Units_Data(void);
// static void Check_Units_Data(void);
void Release_Units_Data(void);

void Capture_Game_Data(void);
void Check_Game_Data(void);
void Release_Game_Data(void);

// void Sanity_Check_Game_Data(void);



#ifdef __cplusplus
}
#endif

#endif /* STU_CHK_H */
