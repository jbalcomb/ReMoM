


need to go through Get_Diplomacy_Statement() and work out the cases
looks like 7 and 9 are deprecated code from MoO1


## Diplomatic Orders

 2: ...have niu_bounty_collect

15: Greet New Wizard - PRS_Maniacal
16: Greet New Wizard - PRS_Ruthless
17: Greet New Wizard - PRS_Aggressive
18: Greet New Wizard - PRS_Chaotic
19: Greet New Wizard - PRS_Lawful
20: Greet New Wizard - PRS_Peaceful





field_6C       ==>  niu_attack_bounty
field_120      ==>  niu_bounty_collect
field_9C       ==>  niu_au_want_tech
field_F0       ==>  niu_au_tech_trade_num
Tribute_Spell  ==>  offer_spell
Tribute_Gold   ==>  offer_gold
Pop_div_10k    ==>  empire_mini_pops
    /* 035A */ struct s_ASTROLOGER astrologer;  /* populated in Record_History() */
    /* 0360 */ uint16_t empire_mini_pops;       /* ¿ mini-pops ?  onlu used for history? */
    /* 0362 */ uint8_t  history[288];           /* ¿ Wizard's Power ? */


Get_Diplomacy_Statement()
    m_diplomacy_message[pos] = G_DIPL_ComposedMessage[itr];
    code = m_diplomacy_message[pos];
    switch(code)
        case 7:
            stu_strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.niu_attack_bounty[player_idx]].name);


MoM
    // Hereafter, DNE in MoO2
    for(itr3 = 0; itr3 < _num_players; itr3++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            _players[itr2].Dipl.DA_Strength[itr3] = 0;
            _players[itr2].Dipl.Dipl_Action[itr3] = do_None;
            if(_players[itr2].Dipl.depr_bounty_collect[itr3] != 0)
            {
                _players[itr2].Dipl.reward_attack_player[itr3] = 0;
                _players[itr2].Dipl.depr_bounty_collect[itr3] = 0;
            }
            _players[itr2].Dipl.niu_au_tech_trade_num[itr3] = 0;
            _players[itr2].Dipl.Tribute_Spell[itr3] = 0;
            _players[itr2].Dipl.niu_au_want_tech[itr3] = 0;
            _players[itr2].Dipl.Tribute_Gold[itr3] = 0;
        }
    }
1oom
    for (player_id_t i = PLAYER_0; i < g->players; ++i) {
        empiretechorbit_t *e = &(g->eto[i]);
        for (player_id_t j = PLAYER_0; j < g->players; ++j) {
            e->diplo_val[j] = 0;
            e->diplo_type[j] = 0;
            if (e->bounty_collect[j] != PLAYER_NONE) {
                e->attack_bounty[j] = PLAYER_NONE;
                e->bounty_collect[j] = PLAYER_NONE;
            }
            e->au_tech_trade_num[j] = 0;
            e->offer_tech[j] = 0;
            e->au_want_tech[j] = 0;
            e->offer_bc[j] = 0;
        }
    }




## MoO1  (1oom)
J:\STU\developp\1oom\src\game\game_turn.c
static void game_turn_update_mood_blunder(struct game_s *g)

## MoM  (ReMoM)
C:\STU\devel\ReMoM\MoM\src\DIPLOMAC.c
// WZD o88p03
// MoO2  Module: DIPLOMAC  Modifier_Diplomacy_Adjustments_()
// 1oom  game_turn.c  game_turn_update_mood_blunder()
void Modifier_Diplomacy_Adjustments(void)

## MoO2  (moo2x)
C:\STU\developp\moo2x-master\STU\MoO2-s_PLAYER.H

