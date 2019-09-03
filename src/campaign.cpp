#include "campaign.h"
#include "globalfuncts.h"
#include "characters.h"
#include "terminal_colors.h"

using namespace std;

extern bool is_random; // for random character gen
extern void save_file();
extern void load_file();
extern bool clearScreens; // a togglable setting

bool quitBuilding = false; // for exiting out of the build character at certain points

/** saveVersion is the build version of current saves.
 * Update saveVersion to the current build version if the saves 
 * have changed in this verison.
 */
const string saveVersion = "6";

string mainMessage;
bool loadSuccess = false;
string loadedFile = "";
Campaign myGame;

void Campaign::pc_menu()
{
  int choice = 0;
  while (choice != 8)
  {
    if (clearScreens)
      simpleClearScreen();
    cout << ">" << mainMessage << "\n\n";
    mainMessage = "";
    cout << GREEN << "---------- CHARACTER MANAGER -----------" << RESET << endl
         << " 1. Build a New Character" << endl
         << " 2. View & Export Characters" << endl
         << " 3. Edit a Character" << endl
         << " 4. Save Current Work" << endl
         << " 5. ";
    if (loadSuccess)
    {
      cout << GREEN << loadedFile << RESET << " file loaded - " << RED << "UNLOAD?" << RESET << endl;
    }
    else
    {
      cout << "Load & Manage Saves" << endl;
    }

    cout << " 6. Delete a Character" << endl;
    cout << " 7. Delete All Characters" << endl;
    cout << " 8. Back to " << CYAN << "MAIN MENU" << RESET << endl;
    cout << GREEN << "----------------------------------------" << RESET << endl;
    choice = getNumber("Choice: ", 1, 8);
    switch (choice)
    {
    case 1:
    {
      if (clearScreens)
        simpleClearScreen();
      cout << "Create Random Character?\n\n";
      cout << "1. Random Character" << endl;
      cout << "2. Manual Character" << endl
           << endl;
      int randomized = getNumber("Choice: ", 1, 2);
      is_random = (randomized == 1) ? true : false;

      int starting_level = 0;

      if (is_random)
      {
        cout << "Pick a level for your random character.\n\n";
        is_random = false;
        starting_level = getNumber("Starting Level(1-20): ", 1, 20);
        is_random = true;
      }

      cout << " Create a New Character! \n\n"
           << "Type Legend: " << YELLOW << "ARCANE " << CYAN << "DIVINE " << RED << "NON CASTER" << RESET << "\n\n"
           << "Character Classes Available:\n\n"
           << CYAN << " 1. Cleric" << CYAN << "      7. Paladin\n"
           << RESET
           << RED << " 2. Fighter" << YELLOW << "     8. Sorcerer\n"
           << RESET
           << RED << " 3. Rogue" << YELLOW << "       9. Bard\n"
           << RESET
           << YELLOW << " 4. Wizard" << RED << "     10. Monk\n"
           << RESET
           << RED << " 5. Barbarian" << CYAN << "  11. Ranger\n"
           << RESET
           << CYAN << " 6. Druid" << YELLOW << "      12. Warlock\n\n"
           << RESET;
      int select_class = getNumberOrQ(1, 12);
      if (select_class == -1)
        return;

      if (!is_random)
      {
        starting_level = getNumber("Starting Level(1-20): ", 1, 20);
      }

      if (clearScreens)
        simpleClearScreen();

      Generic_Character_Class *new_character_build = nullptr;
      switch (select_class)
      {
      case 1:
      {
        new_character_build = new Cleric;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 2:
      {
        new_character_build = new Fighter;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 3:
      {
        new_character_build = new Rogue;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 4:
      {
        new_character_build = new Wizard;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 5:
      {
        new_character_build = new Barbarian;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 6:
      {
        new_character_build = new Druid;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 7:
      {
        new_character_build = new Paladin;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 8:
      {
        new_character_build = new Sorcerer;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 9:
      {
        new_character_build = new Bard;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 10:
      {
        new_character_build = new Monk;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 11:
      {
        new_character_build = new Ranger;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      case 12:
      {
        new_character_build = new Warlock;
        new_character_build->setInitialClassFeatures();
        makecharacter(new_character_build, starting_level);
      }
      break;
      default:
        break;
      }

      is_random = false;
    }
    break;
    case 2:
    { //VIEW & EXPORT CHARACTER SHEET
      if (character_list.size() < 1)
      {
        mainMessage = "Nothing to display. Create characters first.";
      }
      else
      {
        cout << "Current campaign characters:\n\n";
        showCampaignCharacterList();
        bool found = false;
        string valid_name;
        while (!found)
        {
          cout << "VIEW: Enter the character's " << GREEN << "NAME" << RESET << " (leave blank to exit)\n->";
          getline(cin, valid_name);
          if (valid_name.length() == 0)
            break; //break out of while loop, still hits for loop below but wont find anything. todo: improve
          valid_name[0] = toupper(valid_name[0]);
          for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
          {
            if ((*it)->char_name == valid_name)
            {
              found = true;
              break; //leave loop if we found the name, no need to check more
            }
          }
          if (!found)
          {
            cout << "No character named " << valid_name << endl;
          }
        }
        for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
        {
          if (valid_name == (*it)->char_name)
          {
            (*it)->character_sheet();
            char to_export = getYorN("Export/Print Character? (y/n): ");
            if (to_export == 'Y')
            {
              std::string printname = (*it)->char_name + " lvl " + toString((*it)->level) + " " +
                                      (*it)->race + " " + (*it)->char_class;
              (*it)->exportPrint(printname);
              mainMessage = printname + " exported, check exports folder.";
            }
            break; //leave the loop if we found the name, no need to check more
          }
        }
      }
    }
    break;
    case 3:
    { //EDIT/UPDATE CHARACTER
      if (character_list.size() < 1)
      {
        mainMessage = "No characters to Edit. Create some characters first.";
      }
      else
      {
        cout << "Current campaign characters:\n\n";
        showCampaignCharacterList();
        bool found = false;
        string valid_name;
        while (!found)
        {
          cout << "EDIT: Enter the character's " << GREEN << "Name" << RESET << " (leave blank to exit)\n->";
          getline(cin, valid_name);
          if (valid_name.length() == 0)
            break; //break out of while loop, still hits for loop below but wont find anything. todo: improve
          valid_name[0] = toupper(valid_name[0]);
          for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
          {
            if ((*it)->char_name == valid_name)
            {
              found = true;
            }
          }
          if (!found)
            cout << "\nNo character named " << valid_name << ". List of Charcters:\n\n";
        }
        for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
        {
          if (valid_name == (*it)->char_name)
          {
            (*it)->updateCharacter(*this);
            break;
          }
        }
      }
    }
    break;
    case 4:
      save_file();
      break;
    case 5:
      if (!loadSuccess)
      {
        load_file();
      }
      else
      {
        character_list.clear();
        loadedFile = "";
        loadSuccess = false;
      }
      break;
    case 6:
    { //DELETE CHARACTER
      if (character_list.size() < 1)
      {
        mainMessage = "No characters. Nothing to Delete!";
      }
      else
      {
        cout << "Current campaign characters:\n\n";
        showCampaignCharacterList();
        string tmp;
        do
        {
          cout << "DELETE: Enter character's " << GREEN << "NAME" << RESET << " (leave blank to exit)\n->";
          getline(cin, tmp);
          if (tmp.length() == 0)
            break; //break out of do-while loop
          tmp[0] = toupper(tmp[0]);
          for (list<Generic_Character_Class *>::iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
          {
            if ((*it)->char_name == tmp)
            {
              mainMessage = "Deleted " + tmp + ". Goodbye " + tmp + ". You might be missed. *sob*";
              character_list.erase(it);
              tmp = "";
              break;
            }
          }
        } while (tmp != "");
      }
    }
    break;
    case 7:
    { //DELETE ALL THE THINGS
      char sure = getYorN("Are you Sure you want to Delete All Player Characters?(y/n):");
      if (sure == 'Y')
      {
        character_list.clear();
        mainMessage = "All Characters Deleted. Your next save will write this change.";
      }
      else
      {
        mainMessage = "Nothing Deleted.\n\n";
      }
    }
    break;
    default:
      break;
    }
  }
  if (clearScreens)
    simpleClearScreen();
}

bool Campaign::checkname(const string &n) const
{
  for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
  {
    if ((*it)->char_name == n)
    {
      cout << "Name Already Exists.\n";
      return false;
    }
  }
  return true;
}

void Campaign::makecharacter(Generic_Character_Class *new_character, int &starting_level)
{
  new_character->level = starting_level;

  bool goodname = false;

  if (clearScreens)
    simpleClearScreen();
  while (!goodname)
  {
    new_character->setName();
    goodname = checkname(new_character->char_name);
  }

  new_character->setAllStats();

  if (quitBuilding)
  {
    quitBuilding = false;
    delete new_character;
    return;
  }

  new_character->setRace(*new_character);
  new_character->setAlignment();
  new_character->setBackground();
  new_character->setClassDetails(starting_level);
  new_character->setProficiencyBonus();

  character_list.push_back(new_character);
  new_character->initialSkillsSet = true;

  char ans = getYorN("Display Character Sheet now[y/n]?");

  if (ans == 'Y')
  {
    character_list.back()->character_sheet();

    // ASK IF PLAYER WANTS TO EXPORT
    char to_export = getYorN("Export/Print Character? (y/n): ");
    if (to_export == 'Y')
    {
      std::string printname = character_list.back()->char_name + " lvl " + toString(character_list.back()->level) + " " +
                              character_list.back()->race + " " + character_list.back()->char_class;
      new_character->exportPrint(printname);
      mainMessage = printname + " exported, check exports folder. ";
    }
  }
  mainMessage += "Don't forget to 'Save Current Work'";
}

ofstream &Campaign::dumpCharacter(ofstream &os) const
{
  os << saveVersion << endl; // build number to keep saves working correctly
  int charactercount = 0;
  for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
  {
    charactercount++;
    os
        << "saved character: " << charactercount << endl
        //basics
        << (*it)->char_class << endl
        << (*it)->char_name << endl
        << (*it)->race << endl
        << (*it)->alignment << endl
        << (*it)->level << endl

        //class level up menus prep for multiclass functions
        << (*it)->clericlevelupmenus << endl
        << (*it)->fighterlevelupmenus << endl
        << (*it)->roguelevelupmenus << endl
        << (*it)->wizardlevelupmenus << endl
        << (*it)->barbarianlevelupmenus << endl
        << (*it)->druidlevelupmenus << endl
        << (*it)->paladinlevelupmenus << endl
        << (*it)->sorcererlevelupmenus << endl
        << (*it)->bardlevelupmenus << endl
        << (*it)->monklevelupmenus << endl
        << (*it)->rangerlevelupmenus << endl
        << (*it)->warlocklevelupmenus << endl

        << (*it)->backgroundofpc << endl

        << (*it)->move_speed << endl
        << (*it)->fly_speed << endl
        << (*it)->swim_speed << endl
        << (*it)->climb_speed << endl
        << (*it)->hitdicesize << endl
        << (*it)->proficiency_bonus << endl
        << (*it)->strength << endl
        << (*it)->dexterity << endl
        << (*it)->constitution << endl
        << (*it)->intelligence << endl
        << (*it)->wisdom << endl
        << (*it)->charisma << endl
        << (*it)->strSave << endl
        << (*it)->dexSave << endl
        << (*it)->conSave << endl
        << (*it)->intSave << endl
        << (*it)->wisSave << endl
        << (*it)->chaSave << endl
        //class stuff - general
        << "generalclassstuff" << endl
        << (*it)->extra_attack << endl
        << (*it)->expertise << endl
        << (*it)->spellcasting << endl
        << (*it)->timeless_body << endl
        << (*it)->evasion << endl
        << (*it)->unarmored_defense << endl
        << (*it)->fighting_style << endl
        << (*it)->defense << endl
        << (*it)->dueling << endl
        << (*it)->great_weapon_fighting << endl
        << (*it)->protection << endl
        << "cleric" << endl
        << (*it)->destroy_undead << endl //cleric
        << (*it)->channel_divinity << endl
        << (*it)->divine_domain_feature << endl
        << (*it)->divine_intervention << endl
        << (*it)->cleric_cantrips_known << endl
        << (*it)->divine_domain << endl
        << (*it)->arcana_d << endl
        << (*it)->death_d << endl
        << (*it)->knowledge_d << endl
        << (*it)->life_d << endl
        << (*it)->light_d << endl
        << (*it)->nature_d << endl
        << (*it)->tempest_d << endl
        << (*it)->trickery_d << endl
        << (*it)->war_d << endl
        << "fighter" << endl
        << (*it)->action_surge << endl //fighter
        << (*it)->martial_archtype_feature << endl
        << (*it)->indomitable << endl
        << (*it)->eldritch_spells_known << endl
        << (*it)->second_wind << endl
        << (*it)->martial_archtype << endl
        << (*it)->archery << endl
        << (*it)->two_weapon_fighting << endl
        << (*it)->champion << endl
        << (*it)->battle_master << endl
        << (*it)->eldritch_knight << endl
        << (*it)->purple_dragon_knight << endl
        << "barbarian" << endl
        << (*it)->rages << endl //barbarian
        << (*it)->rage_damage << endl
        << (*it)->path_feature << endl
        << (*it)->brutal_critical << endl
        << (*it)->reckless_attack << endl
        << (*it)->danger_sense << endl
        << (*it)->primal_path << endl
        << (*it)->fast_movement << endl
        << (*it)->feral_instinct << endl
        << (*it)->relentless_rage << endl
        << (*it)->persistant_rage << endl
        << (*it)->indomitable_might << endl
        << (*it)->primal_champion << endl
        << (*it)->path_of_the_battlerager << endl
        << (*it)->path_of_the_berseker << endl
        << (*it)->path_of_the_totem_warrior << endl
        << (*it)->bear_totem << endl
        << (*it)->eagle_totem << endl
        << (*it)->wolf_totem << endl
        << "bard" << endl
        << (*it)->bardic_inspiration << endl //bard
        << (*it)->song_of_rest << endl
        << (*it)->bard_college_feature << endl
        << (*it)->magical_secrets << endl
        << (*it)->bard_spells_known << endl
        << (*it)->bard_cantrips_known << endl
        << (*it)->jack_of_all_trades << endl
        << (*it)->bard_college << endl
        << (*it)->font_of_inspiration << endl
        << (*it)->countercharm << endl
        << (*it)->superior_inspiration << endl
        << (*it)->college_of_lore << endl
        << (*it)->college_of_valor << endl
        << (*it)->additional_magical_secrets << endl
        << "druid" << endl
        << (*it)->wild_shape_improvement << endl //druid
        << (*it)->druid_circle_feature << endl
        << (*it)->druid_cantrips_known << endl
        << (*it)->wild_shape << endl
        << (*it)->druid_circle << endl
        << (*it)->beast_spells << endl
        << (*it)->archdruid << endl
        << (*it)->circle_of_the_moon << endl
        << (*it)->circle_of_the_land << endl
        << "monk" << endl
        << (*it)->monastic_tradition_feature << endl //monk
        << (*it)->ki << endl
        << (*it)->unarmored_movement << endl
        << (*it)->martial_arts << endl
        << (*it)->monastic_tradition << endl
        << (*it)->deflect_missles << endl
        << (*it)->slow_fall << endl
        << (*it)->stunning_strike << endl
        << (*it)->ki_empowered_strikes << endl
        << (*it)->stillness_of_mind << endl
        << (*it)->purity_of_body << endl
        << (*it)->tongue_of_the_sun_and_moon << endl
        << (*it)->diamond_soul << endl
        << (*it)->empty_body << endl
        << (*it)->perfect_self << endl
        << (*it)->way_of_the_open_hand << endl
        << (*it)->way_of_the_four_elements << endl
        << (*it)->way_of_the_long_death << endl
        << (*it)->way_of_the_shadow << endl
        << (*it)->way_of_the_sun_soul << endl
        << (*it)->unarmored_movement_improvement << endl
        << "paladin" << endl
        << (*it)->sacred_oath_feature << endl //paladin
        << (*it)->divine_smite << endl
        << (*it)->divine_sense << endl
        << (*it)->lay_on_hands << endl
        << (*it)->divine_health << endl
        << (*it)->sacred_oath << endl
        << (*it)->aura_of_protection << endl
        << (*it)->aura_of_courage << endl
        << (*it)->cleansing_touch << endl
        << (*it)->aura_improvments << endl
        << (*it)->oath_of_ancients << endl
        << (*it)->oath_of_devotion << endl
        << (*it)->oath_of_the_crown << endl
        << (*it)->oath_of_vengeance << endl
        << (*it)->oathbreaker << endl
        << "ranger" << endl
        << (*it)->favored_enemy << endl //ranger
        << (*it)->favored_enemy_languages << endl
        << (*it)->natural_explorer << endl
        << (*it)->ranger_archetype_feature << endl
        << (*it)->ranger_spells_known << endl
        << (*it)->ranger_archetype << endl
        << (*it)->primeval_awareness << endl
        << (*it)->lands_stride << endl
        << (*it)->hide_in_plain_sight << endl
        << (*it)->vanish << endl
        << (*it)->feral_senses << endl
        << (*it)->foe_slayer << endl
        << (*it)->hunter << endl
        << (*it)->beast_master << endl
        << "rogue" << endl
        << (*it)->roguish_archetype_feature << endl //rogue
        << (*it)->arcane_t_spells_known << endl
        << (*it)->sneak_attack << endl
        << (*it)->thieves_cant << endl
        << (*it)->cunning_action << endl
        << (*it)->roguish_archetype << endl
        << (*it)->uncanny_dodge << endl
        << (*it)->reliable_talent << endl
        << (*it)->blindsense << endl
        << (*it)->slippery_mind << endl
        << (*it)->elusive << endl
        << (*it)->stroke_of_luck << endl
        << (*it)->thief << endl
        << (*it)->assassin << endl
        << (*it)->arcane_trickster << endl
        << (*it)->mastermind << endl
        << (*it)->swashbuckler << endl
        << "sorcerer" << endl
        << (*it)->sorcerous_origin_feature << endl //sorcerer
        << (*it)->metamagic << endl
        << (*it)->sorcery_points << endl
        << (*it)->sorcerer_spells_known << endl
        << (*it)->sorcerer_cantrips_known << endl
        << (*it)->sorcerous_origin << endl
        << (*it)->font_of_magic << endl
        << (*it)->sorcerous_restoration << endl
        << (*it)->draconic_bloodline << endl
        << (*it)->wild_magic << endl
        << (*it)->storm_sorcery << endl
        << "warlock" << endl
        << (*it)->warlock_slot_level << endl //warlock
        << (*it)->eldritch_invocations_known << endl
        << (*it)->warlock_spells_known << endl
        << (*it)->warlock_spell_slots << endl
        << (*it)->warlock_cantrips_known << endl
        << (*it)->otherworldly_patron_feature << endl
        << (*it)->mystic_arcanum << endl
        << (*it)->otherworldly_patron << endl
        << (*it)->pact_magic << endl
        << (*it)->pact_boon << endl
        << (*it)->eldritch_master << endl
        << (*it)->archfey << endl
        << (*it)->fiend << endl
        << (*it)->great_old_one << endl
        << (*it)->the_undying << endl
        << "wizard" << endl
        << (*it)->arcane_tradition_feature << endl //wizard
        << (*it)->wizard_cantrips_known << endl
        << (*it)->arcane_recovery << endl
        << (*it)->arcane_tradition << endl
        << (*it)->spell_mastery << endl
        << (*it)->signature_spell << endl
        << (*it)->abjuration << endl
        << (*it)->conjuration << endl
        << (*it)->divination << endl
        << (*it)->enchantment << endl
        << (*it)->evocation << endl
        << (*it)->illusion << endl
        << (*it)->necromancy << endl
        << (*it)->transmutation << endl
        << (*it)->bladesinging << endl
        << "landtypes" << endl //landtypes
        << (*it)->artic << endl
        << (*it)->coast << endl
        << (*it)->desert << endl
        << (*it)->forest << endl
        << (*it)->grassland << endl
        << (*it)->mountain << endl
        << (*it)->swamp << endl
        << (*it)->underdark << endl
        << "creaturetype" << endl //creaturetype
        << (*it)->twohumanoids << endl
        << (*it)->aberrations << endl
        << (*it)->beasts << endl
        << (*it)->celestials << endl
        << (*it)->constructs << endl
        << (*it)->dragons << endl
        << (*it)->elementals << endl
        << (*it)->fey << endl
        << (*it)->fiends << endl
        << (*it)->giants << endl
        << (*it)->monstrosities << endl
        << (*it)->oozes << endl
        << (*it)->plants << endl
        << (*it)->undead << endl
        << "spellslots" << endl //spellslots
        << (*it)->first_ss << endl
        << (*it)->second_ss << endl
        << (*it)->third_ss << endl
        << (*it)->forth_ss << endl
        << (*it)->fifth_ss << endl
        << (*it)->sixth_ss << endl
        << (*it)->seventh_ss << endl
        << (*it)->eighth_ss << endl
        << (*it)->ninth_ss << endl
        << (*it)->warlock_ss << endl
        << "skills" << endl //skills
        << (*it)->initialSkillsSet << endl
        << (*it)->acrobatics << endl
        << (*it)->animal_handling << endl
        << (*it)->arcana << endl
        << (*it)->athletics << endl
        << (*it)->deception << endl
        << (*it)->history << endl
        << (*it)->insight << endl
        << (*it)->intimidation << endl
        << (*it)->investigation << endl
        << (*it)->medicine << endl
        << (*it)->nature << endl
        << (*it)->perception << endl
        << (*it)->performance << endl
        << (*it)->persuasion << endl
        << (*it)->religion << endl
        << (*it)->sleight_of_hand << endl
        << (*it)->stealth << endl
        << (*it)->survival << endl
        << "feats" << endl //feats
        << (*it)->alert << endl
        << (*it)->athlete << endl
        << (*it)->actor << endl
        << (*it)->charger << endl
        << (*it)->crossbow_expert << endl
        << (*it)->defensive_duelist << endl
        << (*it)->dual_wielder << endl
        << (*it)->dungeon_delver << endl
        << (*it)->durable << endl
        << (*it)->elemental_adept << endl
        << (*it)->grappler << endl
        << (*it)->great_weapon_master << endl
        << (*it)->healer << endl
        << (*it)->heavily_armored << endl
        << (*it)->heavy_armor_master << endl
        << (*it)->inspiring_leader << endl
        << (*it)->keen_mind << endl
        << (*it)->lightly_armored << endl
        << (*it)->linguist << endl
        << (*it)->lucky << endl
        << (*it)->mage_slayer << endl
        << (*it)->magic_initiate << endl
        << (*it)->martial_adept << endl
        << (*it)->medium_armor_master << endl
        << (*it)->mobile << endl
        << (*it)->moderately_armored << endl
        << (*it)->mounted_combatant << endl
        << (*it)->observant << endl
        << (*it)->polearm_master << endl
        << (*it)->resilient << endl
        << (*it)->ritual_caster << endl
        << (*it)->savage_attacker << endl
        << (*it)->sentinel << endl
        << (*it)->sharpshooter << endl
        << (*it)->shield_master << endl
        << (*it)->skilled << endl
        << (*it)->skulker << endl
        << (*it)->spell_sniper << endl
        << (*it)->svirfneblin_magic << endl
        << (*it)->tavern_brawler << endl
        << (*it)->tough << endl
        << (*it)->war_caster << endl
        << (*it)->weapon_master << endl
        << "languages" << endl //languages
        << (*it)->abyssal << endl
        << (*it)->aquan << endl
        << (*it)->auran << endl
        << (*it)->celestial << endl
        << (*it)->common << endl
        << (*it)->deep_speech << endl
        << (*it)->draconic << endl
        << (*it)->druidic << endl
        << (*it)->dwarvish << endl
        << (*it)->elvish << endl
        << (*it)->giant << endl
        << (*it)->gnomish << endl
        << (*it)->goblin << endl
        << (*it)->gnoll << endl
        << (*it)->halfling << endl
        << (*it)->infernal << endl
        << (*it)->orc << endl
        << (*it)->primordial << endl
        << (*it)->sylvan << endl
        << (*it)->undercommon << endl
        << "features" << endl //features
        << (*it)->artificers_lore << endl
        << (*it)->brave << endl
        << (*it)->breath_weapon_acid << endl
        << (*it)->breath_weapon_ltg << endl
        << (*it)->breath_weapon_fire << endl
        << (*it)->breath_weapon_poison << endl
        << (*it)->breath_weapon_cold << endl
        << (*it)->darkvision << endl
        << (*it)->devils_tongue << endl
        << (*it)->draconic_ancestry_black << endl
        << (*it)->draconic_ancestry_blue << endl
        << (*it)->draconic_ancestry_brass << endl
        << (*it)->draconic_ancestry_bronze << endl
        << (*it)->draconic_ancestry_copper << endl
        << (*it)->draconic_ancestry_gold << endl
        << (*it)->draconic_ancestry_green << endl
        << (*it)->draconic_ancestry_red << endl
        << (*it)->draconic_ancestry_silver << endl
        << (*it)->draconic_ancestry_white << endl
        << (*it)->drow_magic << endl
        << (*it)->drow_weapon_training << endl
        << (*it)->dwarven_armor_training << endl
        << (*it)->dwarven_combat_training << endl
        << (*it)->dwarven_resilience << endl
        << (*it)->dwarven_toughness << endl
        << (*it)->duergar_resilience << endl
        << (*it)->duergar_magic << endl
        << (*it)->elf_weapon_training << endl
        << (*it)->expert_forgery << endl
        << (*it)->fey_ancestry << endl
        << (*it)->firbolg_magic << endl
        << (*it)->fleet_of_foot << endl
        << (*it)->halflinglucky << endl
        << (*it)->halfling_nimbleness << endl
        << (*it)->hidden_step << endl
        << (*it)->hellfire << endl
        << (*it)->hellish_resistance << endl
        << (*it)->hold_breath << endl
        << (*it)->infernal_legacy << endl
        << (*it)->mask_of_the_wild << endl
        << (*it)->mimicry << endl
        << (*it)->natural_illusionist << endl
        << (*it)->naturally_stealthy << endl
        << (*it)->powerful_build << endl
        << (*it)->silent_speech << endl
        << (*it)->speak_with_small_beasts << endl
        << (*it)->speech_of_beast_and_leaf << endl
        << (*it)->stonecunning << endl
        << (*it)->stone_camouflage << endl
        << (*it)->stones_endurance << endl
        << (*it)->stout_resilience << endl
        << (*it)->superior_darkvision << endl
        << (*it)->trance << endl
        //aasimar stuff
        << (*it)->healing_hands << endl

        << (*it)->light_bearer << endl
        << (*it)->necrotic_shroud << endl
        << (*it)->radiant_consumption << endl
        << (*it)->radiant_soul << endl
        //lizardfolk stuff
        << (*it)->lizardfolk_bite << endl
        << (*it)->cunning_artisan << endl
        << (*it)->natural_armor << endl
        << (*it)->hungry_jaws << endl
        //tabaxi stuff
        << (*it)->feline_agility << endl
        << (*it)->cats_claws << endl
        //triton stuff
        << (*it)->amphibious << endl
        << (*it)->control_air_and_water << endl
        << (*it)->emissary_of_the_sea << endl
        << (*it)->guardian_of_the_depths << endl
        //warforged stuff
        << (*it)->composite_plating << endl
        << (*it)->living_construct << endl
        //changeling stuff
        << (*it)->shapechanger << endl
        //shifter stuff
        << (*it)->shifting << endl
        //tortle stuff
        << (*it)->shell << endl
        << (*it)->retreat_to_shell << endl
        << (*it)->turtle_snapper << endl
        << (*it)->razorback << endl
        << (*it)->razor_fist << endl
        << (*it)->softshell << endl
        << (*it)->nomad << endl
        << (*it)->shell_master << endl
        //aarakocra stuff
        << (*it)->talons << endl
        //genasi stuff
        << (*it)->unending_breath << endl
        << (*it)->mingle_with_the_wind << endl
        << (*it)->earth_walk << endl
        << (*it)->merge_with_stone << endl
        << (*it)->reach_to_the_blaze << endl
        << (*it)->call_to_the_wave << endl

        << "resistances" << endl //resistances
        << (*it)->damage_resist_acid << endl
        << (*it)->damage_resist_ltg << endl
        << (*it)->damage_resist_fire << endl
        << (*it)->damage_resist_poison << endl
        << (*it)->damage_resist_cold << endl
        << (*it)->damage_resist_necrotic << endl
        << (*it)->damage_resist_radiant << endl
        << "disadvantages" << endl //disadvantages
        << (*it)->sunlight_sensitivity << endl
        << "tools" << endl //artisan tools & supplies
        << (*it)->alchemist << endl
        << (*it)->brewer << endl
        << (*it)->calligrapher << endl
        << (*it)->carpenter << endl
        << (*it)->cartographer << endl
        << (*it)->cobbler << endl
        << (*it)->cook << endl
        << (*it)->glassblower << endl
        << (*it)->jeweler << endl
        << (*it)->leatherworker << endl
        << (*it)->mason << endl
        << (*it)->painter << endl
        << (*it)->potter << endl
        << (*it)->smith << endl
        << (*it)->tinker << endl
        << (*it)->weaver << endl
        << (*it)->woodcarver << endl
        << "kits" << endl //kits & tools
        << (*it)->disguise << endl
        << (*it)->forgery << endl
        << (*it)->herbalism << endl
        << (*it)->navigator << endl
        << (*it)->poisoner << endl
        << (*it)->thieves << endl
        << (*it)->vehicle << endl
        << "musicalinstruments" << endl //musical instruments
        << (*it)->bagpipes << endl
        << (*it)->drum << endl
        << (*it)->dulcimer << endl
        << (*it)->flute << endl
        << (*it)->lute << endl
        << (*it)->lyre << endl
        << (*it)->horn << endl
        << (*it)->pan_flute << endl
        << (*it)->shawm << endl
        << (*it)->viol << endl;
  }
  return os;
}

bool Campaign::retrieveCharacter(ifstream &ins)
{
  bool debugRetrieve = false; // change to true to see the console logs

  static string charClassTempVar;
  static string sbuffer;
  static int charBackgroundProcessor;
  static Generic_Character_Class *v;

  //----------- LOAD IN SAVE VERSION NUMBER --------//
  getline(ins, sbuffer);

  if (debugRetrieve)
    cout << "version retrieved is: " << sbuffer << endl;

  if (sbuffer.compare(saveVersion) != 0)
  {
    cout << "Versions Do Not Match\n";
    return false;
  }

  //---------- GET WHICH SAVED CHARACTER # ---------//
  getline(ins, sbuffer);

  do
  {

    if (debugRetrieve)
      cout << "Loading in " << sbuffer << endl;

    //--------- LOAD IN CHARACTER CLASS ----------//
    getline(ins, charClassTempVar);
    if (charClassTempVar == "Cleric")
      v = new Cleric;
    else if (charClassTempVar == "Fighter")
      v = new Fighter;
    else if (charClassTempVar == "Barbarian")
      v = new Barbarian;
    else if (charClassTempVar == "Bard")
      v = new Bard;
    else if (charClassTempVar == "Druid")
      v = new Druid;
    else if (charClassTempVar == "Monk")
      v = new Monk;
    else if (charClassTempVar == "Paladin")
      v = new Paladin;
    else if (charClassTempVar == "Ranger")
      v = new Ranger;
    else if (charClassTempVar == "Rogue")
      v = new Rogue;
    else if (charClassTempVar == "Sorcerer")
      v = new Sorcerer;
    else if (charClassTempVar == "Warlock")
      v = new Warlock;
    else if (charClassTempVar == "Wizard")
      v = new Wizard;
    else
      return false;
    v->char_class = charClassTempVar;

    if (debugRetrieve)
      cout << "class set:  " << charClassTempVar << endl;

    //----- LOAD IN NAME, RACE, ALIGNMENT -------//
    getline(ins, v->char_name);
    getline(ins, v->race);
    getline(ins, v->alignment);

    if (debugRetrieve)
    {
      cout << "name of char: " << v->char_name << endl;
      cout << "race set: " << v->race << endl;
    }

    //------ LOAD IN LEVELS SET ----------//
    ins >> v->level;
    ins >> v->clericlevelupmenus;
    ins >> v->fighterlevelupmenus;
    ins >> v->roguelevelupmenus;
    ins >> v->wizardlevelupmenus;
    ins >> v->barbarianlevelupmenus;
    ins >> v->druidlevelupmenus;
    ins >> v->paladinlevelupmenus;
    ins >> v->sorcererlevelupmenus;
    ins >> v->bardlevelupmenus;
    ins >> v->monklevelupmenus;
    ins >> v->rangerlevelupmenus;
    ins >> v->warlocklevelupmenus;

    if (debugRetrieve)
      cout << "level up menus set" << endl;

    //------------- LOAD IN BACKGROUND ------------------//
    ins >> charBackgroundProcessor;
    v->backgroundofpc = static_cast<enum Generic_Character_Class::characterbackground>(charBackgroundProcessor);

    if (debugRetrieve)
      cout << "background set to: " << charBackgroundProcessor << endl;

    //----------- LOAD IN BASIC STAT BLOCK -----------//
    ins >> v->move_speed;
    ins >> v->fly_speed;
    ins >> v->swim_speed;
    ins >> v->climb_speed;
    ins >> v->hitdicesize;
    ins >> v->proficiency_bonus;
    ins >> v->strength;
    ins >> v->dexterity;
    ins >> v->constitution;
    ins >> v->intelligence;
    ins >> v->wisdom;
    ins >> v->charisma;
    ins >> v->strSave;
    ins >> v->dexSave;
    ins >> v->conSave;
    ins >> v->intSave;
    ins >> v->wisSave;
    ins >> v->chaSave;

    if (debugRetrieve)
      cout << "basic abiliity stats" << endl;
    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN GENERAL CLASS STUFF ---------//
    ins >> v->extra_attack;
    ins >> v->expertise;
    ins >> v->spellcasting;
    ins >> v->timeless_body;
    ins >> v->evasion;
    ins >> v->unarmored_defense;
    ins >> v->fighting_style;
    ins >> v->defense;
    ins >> v->dueling;
    ins >> v->great_weapon_fighting;
    ins >> v->protection;

    if (debugRetrieve)
      cout << "general class stuff set" << endl;

    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN CLERIC CLASS ------------//
    ins >> v->destroy_undead;
    ins >> v->channel_divinity;
    ins >> v->divine_domain_feature;
    ins >> v->divine_intervention;
    ins >> v->cleric_cantrips_known;
    ins >> v->divine_domain;
    ins >> v->arcana_d;
    ins >> v->death_d;
    ins >> v->knowledge_d;
    ins >> v->life_d;
    ins >> v->light_d;
    ins >> v->nature_d;
    ins >> v->tempest_d;
    ins >> v->trickery_d;
    ins >> v->war_d;

    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN FIGHTER CLASS -------------//
    ins >> v->action_surge;
    ins >> v->martial_archtype_feature;
    ins >> v->indomitable;
    ins >> v->eldritch_spells_known;
    ins >> v->second_wind;
    ins >> v->martial_archtype;
    ins >> v->archery;
    ins >> v->two_weapon_fighting;
    ins >> v->champion;
    ins >> v->battle_master;
    ins >> v->eldritch_knight;
    ins >> v->purple_dragon_knight;

    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN BARBARIAN CLASS -------------//
    ins >> v->rages;
    ins >> v->rage_damage;
    ins >> v->path_feature;
    ins >> v->brutal_critical;
    ins >> v->reckless_attack;
    ins >> v->danger_sense;
    ins >> v->primal_path;
    ins >> v->fast_movement;
    ins >> v->feral_instinct;
    ins >> v->relentless_rage;
    ins >> v->persistant_rage;
    ins >> v->indomitable_might;
    ins >> v->primal_champion;
    ins >> v->path_of_the_battlerager;
    ins >> v->path_of_the_berseker;
    ins >> v->path_of_the_totem_warrior;
    ins >> v->bear_totem;
    ins >> v->eagle_totem;
    ins >> v->wolf_totem;

    ins.get();
    getline(ins, sbuffer);

    //---------- LOAD IN BARD CLASS ------------//
    ins >> v->bardic_inspiration;
    ins >> v->song_of_rest;
    ins >> v->bard_college_feature;
    ins >> v->magical_secrets;
    ins >> v->bard_spells_known;
    ins >> v->bard_cantrips_known;
    ins >> v->jack_of_all_trades;
    ins >> v->bard_college;
    ins >> v->font_of_inspiration;
    ins >> v->countercharm;
    ins >> v->superior_inspiration;
    ins >> v->college_of_lore;
    ins >> v->college_of_valor;
    ins >> v->additional_magical_secrets;

    ins.get();
    getline(ins, sbuffer);

    //----------- LOAD IN DRUID CLASS ----------//
    ins >> v->wild_shape_improvement;
    ins >> v->druid_circle_feature;
    ins >> v->druid_cantrips_known;
    ins >> v->wild_shape;
    ins >> v->druid_circle;
    ins >> v->beast_spells;
    ins >> v->archdruid;
    ins >> v->circle_of_the_moon;
    ins >> v->circle_of_the_land;

    ins.get();
    getline(ins, sbuffer);

    //---------- LOAD IN MONK CLASS -----------//
    ins >> v->monastic_tradition_feature;
    ins >> v->ki;
    ins >> v->unarmored_movement;
    ins >> v->martial_arts;
    ins >> v->monastic_tradition;
    ins >> v->deflect_missles;
    ins >> v->slow_fall;
    ins >> v->stunning_strike;
    ins >> v->ki_empowered_strikes;
    ins >> v->stillness_of_mind;
    ins >> v->purity_of_body;
    ins >> v->tongue_of_the_sun_and_moon;
    ins >> v->diamond_soul;
    ins >> v->empty_body;
    ins >> v->perfect_self;
    ins >> v->way_of_the_open_hand;
    ins >> v->way_of_the_four_elements;
    ins >> v->way_of_the_long_death;
    ins >> v->way_of_the_shadow;
    ins >> v->way_of_the_sun_soul;
    ins >> v->unarmored_movement_improvement;

    ins.get();
    getline(ins, sbuffer);

    //-------- LOAD IN PALADIN CLASS --------//
    ins >> v->sacred_oath_feature;
    ins >> v->divine_smite;
    ins >> v->divine_sense;
    ins >> v->lay_on_hands;
    ins >> v->divine_health;
    ins >> v->sacred_oath;
    ins >> v->aura_of_protection;
    ins >> v->aura_of_courage;
    ins >> v->cleansing_touch;
    ins >> v->aura_improvments;
    ins >> v->oath_of_ancients;
    ins >> v->oath_of_devotion;
    ins >> v->oath_of_the_crown;
    ins >> v->oath_of_vengeance;
    ins >> v->oathbreaker;

    ins.get();
    getline(ins, sbuffer);

    //-------- LOAD IN RANGER CLASS ----------//
    ins >> v->favored_enemy;
    ins >> v->favored_enemy_languages;
    ins >> v->natural_explorer;
    ins >> v->ranger_archetype_feature;
    ins >> v->ranger_spells_known;
    ins >> v->ranger_archetype;
    ins >> v->primeval_awareness;
    ins >> v->lands_stride;
    ins >> v->hide_in_plain_sight;
    ins >> v->vanish;
    ins >> v->feral_senses;
    ins >> v->foe_slayer;
    ins >> v->hunter;
    ins >> v->beast_master;

    ins.get();
    getline(ins, sbuffer);

    //-------- LOAD IN ROGUE CLASS ----------//
    ins >> v->roguish_archetype_feature;
    ins >> v->arcane_t_spells_known;
    ins >> v->sneak_attack;
    ins >> v->thieves_cant;
    ins >> v->cunning_action;
    ins >> v->roguish_archetype;
    ins >> v->uncanny_dodge;
    ins >> v->reliable_talent;
    ins >> v->blindsense;
    ins >> v->slippery_mind;
    ins >> v->elusive;
    ins >> v->stroke_of_luck;
    ins >> v->thief;
    ins >> v->assassin;
    ins >> v->arcane_trickster;
    ins >> v->mastermind;
    ins >> v->swashbuckler;

    ins.get();
    getline(ins, sbuffer);

    //-------- LOAD IN SORCERER CLASS ----------//
    ins >> v->sorcerous_origin_feature;
    ins >> v->metamagic;
    ins >> v->sorcery_points;
    ins >> v->sorcerer_spells_known;
    ins >> v->sorcerer_cantrips_known;
    ins >> v->sorcerous_origin;
    ins >> v->font_of_magic;
    ins >> v->sorcerous_restoration;
    ins >> v->draconic_bloodline;
    ins >> v->wild_magic;
    ins >> v->storm_sorcery;

    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN WARLOCK CLASS --------//
    ins >> v->warlock_slot_level;
    ins >> v->eldritch_invocations_known;
    ins >> v->warlock_spells_known;
    ins >> v->warlock_spell_slots;
    ins >> v->warlock_cantrips_known;
    ins >> v->otherworldly_patron_feature;
    ins >> v->mystic_arcanum;
    ins >> v->otherworldly_patron;
    ins >> v->pact_magic;
    ins >> v->pact_boon;
    ins >> v->eldritch_master;
    ins >> v->archfey;
    ins >> v->fiend;
    ins >> v->great_old_one;
    ins >> v->the_undying;

    ins.get();
    getline(ins, sbuffer);

    //--------- LOAD IN WIZARD CLASS -------//
    ins >> v->arcane_tradition_feature;
    ins >> v->wizard_cantrips_known;
    ins >> v->arcane_recovery;
    ins >> v->arcane_tradition;
    ins >> v->spell_mastery;
    ins >> v->signature_spell;
    ins >> v->abjuration;
    ins >> v->conjuration;
    ins >> v->divination;
    ins >> v->enchantment;
    ins >> v->evocation;
    ins >> v->illusion;
    ins >> v->necromancy;
    ins >> v->transmutation;
    ins >> v->bladesinging;

    if (debugRetrieve)
      cout << "precise class stuff set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //------- LOAD IN RANGER LANDTYPES --------//
    ins >> v->artic;
    ins >> v->coast;
    ins >> v->desert;
    ins >> v->forest;
    ins >> v->grassland;
    ins >> v->mountain;
    ins >> v->swamp;
    ins >> v->underdark;

    if (debugRetrieve)
      cout << "landtypes set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //------ LOAD IN FAVORED ENEMY CREATURETYPS ------//
    ins >> v->twohumanoids;
    ins >> v->aberrations;
    ins >> v->beasts;
    ins >> v->celestials;
    ins >> v->constructs;
    ins >> v->dragons;
    ins >> v->elementals;
    ins >> v->fey;
    ins >> v->fiends;
    ins >> v->giants;
    ins >> v->monstrosities;
    ins >> v->oozes;
    ins >> v->plants;
    ins >> v->undead;

    if (debugRetrieve)
      cout << "creaturetype set " << endl;

    ins.get();
    getline(ins, sbuffer);

    //------- LOAD IN SPELLSLOTS ------//
    ins >> v->first_ss;
    ins >> v->second_ss;
    ins >> v->third_ss;
    ins >> v->forth_ss;
    ins >> v->fifth_ss;
    ins >> v->sixth_ss;
    ins >> v->seventh_ss;
    ins >> v->eighth_ss;
    ins >> v->ninth_ss;
    ins >> v->warlock_ss;

    if (debugRetrieve)
      cout << "spellslots set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //------- LOAD IN SKILLS ------//
    ins >> v->initialSkillsSet;
    ins >> v->acrobatics;
    ins >> v->animal_handling;
    ins >> v->arcana;
    ins >> v->athletics;
    ins >> v->deception;
    ins >> v->history;
    ins >> v->insight;
    ins >> v->intimidation;
    ins >> v->investigation;
    ins >> v->medicine;
    ins >> v->nature;
    ins >> v->perception;
    ins >> v->performance;
    ins >> v->persuasion;
    ins >> v->religion;
    ins >> v->sleight_of_hand;
    ins >> v->stealth;
    ins >> v->survival;

    if (debugRetrieve)
      cout << "skills set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //------ LOAD IN FEATS ------//
    ins >> v->alert;
    ins >> v->athlete;
    ins >> v->actor;
    ins >> v->charger;
    ins >> v->crossbow_expert;
    ins >> v->defensive_duelist;
    ins >> v->dual_wielder;
    ins >> v->dungeon_delver;
    ins >> v->durable;
    ins >> v->elemental_adept;
    ins >> v->grappler;
    ins >> v->great_weapon_master;
    ins >> v->healer;
    ins >> v->heavily_armored;
    ins >> v->heavy_armor_master;
    ins >> v->inspiring_leader;
    ins >> v->keen_mind;
    ins >> v->lightly_armored;
    ins >> v->linguist;
    ins >> v->lucky;
    ins >> v->mage_slayer;
    ins >> v->magic_initiate;
    ins >> v->martial_adept;
    ins >> v->medium_armor_master;
    ins >> v->mobile;
    ins >> v->moderately_armored;
    ins >> v->mounted_combatant;
    ins >> v->observant;
    ins >> v->polearm_master;
    ins >> v->resilient;
    ins >> v->ritual_caster;
    ins >> v->savage_attacker;
    ins >> v->sentinel;
    ins >> v->sharpshooter;
    ins >> v->shield_master;
    ins >> v->skilled;
    ins >> v->skulker;
    ins >> v->spell_sniper;
    ins >> v->svirfneblin_magic;
    ins >> v->tavern_brawler;
    ins >> v->tough;
    ins >> v->war_caster;
    ins >> v->weapon_master;

    if (debugRetrieve)
      cout << "feats set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //------ LOAD IN LANGUAGES ------//
    ins >> v->abyssal;
    ins >> v->aquan;
    ins >> v->auran;
    ins >> v->celestial;
    ins >> v->common;
    ins >> v->deep_speech;
    ins >> v->draconic;
    ins >> v->druidic;
    ins >> v->dwarvish;
    ins >> v->elvish;
    ins >> v->giant;
    ins >> v->gnomish;
    ins >> v->goblin;
    ins >> v->gnoll;
    ins >> v->halfling;
    ins >> v->infernal;
    ins >> v->orc;
    ins >> v->primordial;
    ins >> v->sylvan;
    ins >> v->undercommon;

    if (debugRetrieve)
      cout << "languages set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //----- LOAD IN RACE AND OTHER FEATURES -----//
    ins >> v->artificers_lore;
    ins >> v->brave;
    ins >> v->breath_weapon_acid;
    ins >> v->breath_weapon_ltg;
    ins >> v->breath_weapon_fire;
    ins >> v->breath_weapon_poison;
    ins >> v->breath_weapon_cold;
    ins >> v->darkvision;
    ins >> v->devils_tongue;
    ins >> v->draconic_ancestry_black;
    ins >> v->draconic_ancestry_blue;
    ins >> v->draconic_ancestry_brass;
    ins >> v->draconic_ancestry_bronze;
    ins >> v->draconic_ancestry_copper;
    ins >> v->draconic_ancestry_gold;
    ins >> v->draconic_ancestry_green;
    ins >> v->draconic_ancestry_red;
    ins >> v->draconic_ancestry_silver;
    ins >> v->draconic_ancestry_white;
    ins >> v->drow_magic;
    ins >> v->drow_weapon_training;
    ins >> v->dwarven_armor_training;
    ins >> v->dwarven_combat_training;
    ins >> v->dwarven_resilience;
    ins >> v->dwarven_toughness;
    ins >> v->duergar_resilience;
    ins >> v->duergar_magic;
    ins >> v->elf_weapon_training;
    ins >> v->expert_forgery;
    ins >> v->fey_ancestry;
    ins >> v->firbolg_magic;
    ins >> v->fleet_of_foot;
    ins >> v->halflinglucky;
    ins >> v->halfling_nimbleness;
    ins >> v->hidden_step;
    ins >> v->hellfire;
    ins >> v->hellish_resistance;
    ins >> v->hold_breath;
    ins >> v->infernal_legacy;
    ins >> v->mask_of_the_wild;
    ins >> v->mimicry;
    ins >> v->natural_illusionist;
    ins >> v->naturally_stealthy;
    ins >> v->powerful_build;
    ins >> v->silent_speech;
    ins >> v->speak_with_small_beasts;
    ins >> v->speech_of_beast_and_leaf;
    ins >> v->stonecunning;
    ins >> v->stone_camouflage;
    ins >> v->stones_endurance;
    ins >> v->stout_resilience;
    ins >> v->superior_darkvision;
    ins >> v->trance;
    //aasimar stuff
    ins >> v->healing_hands;
    ins >> v->light_bearer;
    ins >> v->necrotic_shroud;
    ins >> v->radiant_consumption;
    ins >> v->radiant_soul;
    //lizardfolk stuff
    ins >> v->lizardfolk_bite;
    ins >> v->cunning_artisan;
    ins >> v->natural_armor;
    ins >> v->hungry_jaws;
    //tabaxi stuff
    ins >> v->feline_agility;
    ins >> v->cats_claws;
    //triton stuff
    ins >> v->amphibious;
    ins >> v->control_air_and_water;
    ins >> v->emissary_of_the_sea;
    ins >> v->guardian_of_the_depths;
    //warforged stuff
    ins >> v->composite_plating;
    ins >> v->living_construct;
    //changeling stuff
    ins >> v->shapechanger;
    //shifter stuff
    ins >> v->shifting;
    //tortle
    ins >> v->shell;
    ins >> v->retreat_to_shell;
    ins >> v->turtle_snapper;
    ins >> v->razorback;
    ins >> v->razor_fist;
    ins >> v->softshell;
    ins >> v->nomad;
    ins >> v->shell_master;
    //Aarakocra stuff
    ins >> v->talons;
    //genasi stuff
    ins >> v->unending_breath;
    ins >> v->mingle_with_the_wind;
    ins >> v->earth_walk;
    ins >> v->merge_with_stone;
    ins >> v->reach_to_the_blaze;
    ins >> v->call_to_the_wave;

    if (debugRetrieve)
      cout << "character features set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //----- LOAD IN RESISTANCES -----//
    ins >> v->damage_resist_acid;
    ins >> v->damage_resist_ltg;
    ins >> v->damage_resist_fire;
    ins >> v->damage_resist_poison;
    ins >> v->damage_resist_cold;
    ins >> v->damage_resist_necrotic;
    ins >> v->damage_resist_radiant;

    if (debugRetrieve)
      cout << "resistances set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //---- LOAD IN DISADVANTAGES ----//
    ins >> v->sunlight_sensitivity;

    if (debugRetrieve)
      cout << "disadvantages set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //----- LOAD IN ARTISAN TOOLS -----//
    ins >> v->alchemist;
    ins >> v->brewer;
    ins >> v->calligrapher;
    ins >> v->carpenter;
    ins >> v->cartographer;
    ins >> v->cobbler;
    ins >> v->cook;
    ins >> v->glassblower;
    ins >> v->jeweler;
    ins >> v->leatherworker;
    ins >> v->mason;
    ins >> v->painter;
    ins >> v->potter;
    ins >> v->smith;
    ins >> v->tinker;
    ins >> v->weaver;
    ins >> v->woodcarver;

    if (debugRetrieve)
      cout << "artisan tools and supplies set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //----- LOAD IN KITS ------//
    ins >> v->disguise;
    ins >> v->forgery;
    ins >> v->herbalism;
    ins >> v->navigator;
    ins >> v->poisoner;
    ins >> v->thieves;
    ins >> v->vehicle;

    if (debugRetrieve)
      cout << "kits and tools set" << endl;
    ins.get();
    getline(ins, sbuffer);

    //----- LOAD IN MUSICAL INSTRUMENTS -----//
    ins >> v->bagpipes;
    ins >> v->drum;
    ins >> v->dulcimer;
    ins >> v->flute;
    ins >> v->lute;
    ins >> v->lyre;
    ins >> v->horn;
    ins >> v->pan_flute;
    ins >> v->shawm;
    ins >> v->viol;

    if (debugRetrieve)
      cout << "musical instruments set" << endl;

    character_list.push_back(v);

    ins.get();
  } while (getline(ins, sbuffer));
  return true;
}

void Campaign::showCampaignCharacterList() const
{
  for (list<Generic_Character_Class *>::const_iterator it = this->character_list.begin(); it != this->character_list.end(); ++it)
  {
    cout << (*it)->char_name << " - " << (*it)->race << " " << (*it)->char_class << "(" << (*it)->level << ")" << endl;
  }
  cout << endl;
}
