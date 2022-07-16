//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef doctorStrange_h
#define doctorStrange_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/// STUDENT'S ANSWER BEGINS HERE
/// STUDENTS CAN ADD HELPER FUNCTIONS

struct Livitation {
  bool has_Livitation;
  int resist_damage;

  Livitation() {
    this->has_Livitation = false;
    this->resist_damage = 3;
  }

  void assign(bool has_Livitation, int resist_damage) {
    this->has_Livitation = has_Livitation;
    this->resist_damage = resist_damage;
  }
};

struct Doctor_Strange {
  int MAX_HP;
  int HP;
  int LV;
  int EXP;
  int TS;
  Livitation livitation;

  Doctor_Strange(int hp, int lv, int exp, int ts) {
    this->MAX_HP = hp;
    this->HP = hp;
    this->LV = lv;
    this->EXP = exp;
    this->TS = ts;
  }
};

std::string tokenize(std::string events, int &start, int &event_th,
                     std::string del = "#") {
  int end = (int)events.find(del, start);
  std::string result = "";
  if (end != -1) {
    result = events.substr(start, end - start);
    start = end + 1;
  } else {
    result = events.substr(start, events.size() - 1 - start);
    start = events.size();
  }
  event_th++;
  return result;
}

int get_event_code(std::string str, std::string &witchcraft,
                   std::string del = " ") {
  int end = str.find(del);
  witchcraft = str.substr(end + 1, str.size());
  return stoi(str.substr(0, end));
}

void calculate_exp_lv_hp_maxhp(Doctor_Strange &doctor_strange, int exp) {
  while (exp > 0) {
    doctor_strange.EXP += exp > 100 ? 100 : exp;
    if (doctor_strange.EXP > 100) {
      if (doctor_strange.LV < 10) {
        doctor_strange.EXP -= 100;
        doctor_strange.LV += 1;
        doctor_strange.MAX_HP += 50;
        doctor_strange.HP += 10;
        if (doctor_strange.HP > doctor_strange.MAX_HP) {
          doctor_strange.HP = doctor_strange.MAX_HP;
        }
        if (doctor_strange.MAX_HP > 999) {
          doctor_strange.MAX_HP = 999;
        }
        if (doctor_strange.HP > 999) {
          doctor_strange.HP = 999;
        }
      } else {
        doctor_strange.EXP = 100;
        doctor_strange.LV = 10;
      }
    }
    exp -= 100;
  }
}

void number_of_witchcraft(std::string witchcraft, std::string spell,
                          int index_witchcraft, int index_spell, int &count) {
  while (index_witchcraft < witchcraft.length()) {
    if (std::tolower(witchcraft[index_witchcraft]) ==
        std::tolower(spell[index_spell])) {
      if (index_spell == spell.length() - 1) {
        count++;
        return;
      }
      number_of_witchcraft(witchcraft, spell, index_witchcraft + 1,
                           index_spell + 1, count);
    }
    index_witchcraft++;
  }
}

bool check_prime(int n) {
  if (n < 2)
    return false;

  for (int i = 2; i < sqrt(n); i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}

int nearest_hp_prime(int hp) {
  while (hp < 999) {
    if (check_prime(hp))
      return hp;
    hp++;
  }
  return -1;
}

int handleEvents(string &HP, string &LV, string &EXP, string &TS,
                 string &events) {
  /// Students have to complete this function and DO NOT modify any parameters
  /// in this function.
  int result = 0;
  cout << HP << " " << LV << " " << EXP << " " << TS << endl;
  cout << events << endl;
  int event_size = (int)events.size();

  // if (events[0] != '!' || events[event_size - 2] != '!')
  //   return -1;

  if (events[0] != '!')
    return -1;

  int event_th = 0;
  int start = 1;
  Doctor_Strange doctor_strange(stoi(HP), stoi(LV), stoi(EXP), stoi(TS));

  do {
    string str = tokenize(events, start, event_th);
    string witchcraft = "";
    int event_code = get_event_code(str, witchcraft, " ");

    // cout << event_code << endl;
    switch (event_code) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
      int b = event_th % 10;
      int LV0 = event_th > 6 ? (b > 5 ? b : 5) : b;
      int nearest_prime = nearest_hp_prime(doctor_strange.HP);
      int g_function = (event_th + nearest_prime) % 100;
      // cout << "b: " << b << endl;
      // cout << "LV0: " << LV0 << endl;
      int mon_exp = event_code == 1   ? 10
                    : event_code == 2 ? 20
                    : event_code == 3 ? 40
                    : event_code == 4 ? 50
                    : event_code == 5 ? 70
                                      : 0;
      float base_damage = event_code == 1   ? 1.5
                          : event_code == 2 ? 2.5
                          : event_code == 3 ? 4.5
                          : event_code == 4 ? 7.5
                          : event_code == 5 ? 9.5
                                            : 0;
      if (doctor_strange.LV < LV0) {
        int damage = 0;
        if (doctor_strange.livitation.has_Livitation) {
          if (doctor_strange.livitation.resist_damage > 0) {
            damage = base_damage * LV0 * 10 * (100 - g_function) / 100;
            doctor_strange.livitation.resist_damage--;

          } else {
            if (doctor_strange.LV < 3) {
              doctor_strange.LV = 1;
            }
          }
        } else {
          damage = base_damage * LV0 * 10;
        }
        doctor_strange.HP -= damage;
        if (doctor_strange.HP <= 0) {
          if (doctor_strange.TS > 0) {
            doctor_strange.HP = doctor_strange.MAX_HP;
            doctor_strange.TS--;
          } else {
            return -1;
          }
        }
      } else {
        int exp = doctor_strange.LV == LV0 ? mon_exp / 2 : mon_exp;
        calculate_exp_lv_hp_maxhp(doctor_strange, exp);
        // cout << "doctor_strange.EXP: " << doctor_strange.EXP << endl;
      }
      break;
    }
    case 6: {
      int nearest_prime = nearest_hp_prime(doctor_strange.HP);
      int g_function = (event_th + nearest_prime) % 100;
      int count_attack = 0;
      int count_defence = 0;
      number_of_witchcraft(witchcraft, "attack", 0, 0, count_attack);
      number_of_witchcraft(witchcraft, "defense", 0, 0, count_defence);

      bool check_livitation = doctor_strange.livitation.resist_damage > 0 &&
                              doctor_strange.livitation.has_Livitation;
      if (check_livitation) {
        doctor_strange.livitation.resist_damage--;
      }
      int blood_loss_reduction_rate = check_livitation
                                          ? count_defence * 10 + g_function
                                          : count_defence * 10;
      int winning_rate =
          check_livitation ? count_attack * 10 + g_function : count_attack * 10;
      int gargantos_attack = (event_code + witchcraft.size()) % 100;

      cout << "winning_rate: " << winning_rate << endl;
      cout << "blood_loss_reduction_rate: " << blood_loss_reduction_rate
           << endl;
      cout << "gargantos_attack: " << gargantos_attack << endl;
      if (winning_rate > gargantos_attack) {
        doctor_strange.TS += 1;
        calculate_exp_lv_hp_maxhp(doctor_strange, 200);
      } else {
        if (blood_loss_reduction_rate < 100) {
          if (doctor_strange.HP >= 100) {
            doctor_strange.HP -=
                doctor_strange.HP * (100 - blood_loss_reduction_rate) / 100;

          } else {
            doctor_strange.HP = 0;
            return -1;
          }
        }
      }
      break;
    }
    case 7: {
      if (doctor_strange.livitation.resist_damage > 0) {
        if (!doctor_strange.livitation.has_Livitation) {
          doctor_strange.livitation.assign(true, 3);
          doctor_strange.LV += 2;
          if (doctor_strange.LV > 10) {
            doctor_strange.LV = 10;
          }
        }
      }
      break;
    }

    default: {
      std::cout << "default is running" << endl;
      break;
    }
    }

    cout << doctor_strange.MAX_HP << " " << doctor_strange.HP << " "
         << doctor_strange.LV << " " << doctor_strange.EXP << " "
         << doctor_strange.TS << endl;
      cout << doctor_strange.livitation.has_Livitation << " " << doctor_strange.livitation.resist_damage << " ";
  } while (start != event_size);
  result += doctor_strange.TS + doctor_strange.LV + doctor_strange.EXP +
            doctor_strange.HP;
  return result;
}

/// END OF STUDENT'S ANSWER

#endif /* studyInPink_h */
