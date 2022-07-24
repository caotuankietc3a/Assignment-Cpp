//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef doctorStrange_h
#define doctorStrange_h

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/// STUDENT'S ANSWER BEGINS HERE
/// STUDENTS CAN ADD HELPER FUNCTIONS
int MIN_INT = -2147483648;
int COL_DEFENSE_LAYER = 7;
int ROW_DEFENSE_LAYER = 7;

std::string tokenize(std::string events, int &start, std::string del = "#");
int get_event_code(std::string str, std::string &witchcraft,
                   std::string del = " ");
bool check_int_code(std::string code);
int tokenize_door(std::string str, int &start, std::string del = " ");

struct Livitation {
  bool has_Livitation;
  bool is_real;
  int resist_damage;
  bool is_deprived;
  int increased_LV;

  Livitation() {
    this->has_Livitation = false;
    this->is_real = false;
    this->is_deprived = false;
    this->resist_damage = 0;
    this->increased_LV = 0;
  }

  void assign(bool has_Livitation, int resist_damage, bool is_real) {
    this->has_Livitation = has_Livitation;
    this->is_real = is_real;
    this->resist_damage = resist_damage;
  }

  bool check_livitation() {
    return this->has_Livitation && this->is_real && this->resist_damage > 0;
  }

  void reset_livitation() {
    this->has_Livitation = this->has_Livitation ? true : false;
    this->is_real = true;
    this->resist_damage = 3;
  }

  void reset_real_Livitation() { this->is_real = true; }

  void deprived_Livitation() {
    this->is_deprived = true;
    this->has_Livitation = false;
  }

  void undeprived_Livitation() {
    this->is_deprived = false;
    this->has_Livitation = true;
  }

  void store_increased_LV(int lv) { this->increased_LV = lv; }
};

struct Wong {
  int harm_help_times;
  bool is_real;
  bool meeting;
  bool returned_to_KamarTaj;
  Wong() {
    this->harm_help_times = 0;
    this->is_real = true;
    this->meeting = false;
    this->returned_to_KamarTaj = false;
  }

  void meeting_real_Wong() {
    // std::cout << "sssssssssssssdddddddddddddddddddd" << std::endl;
    this->is_real = true;
    this->meeting = true;
    this->harm_help_times = 3;
  }

  void meeting_fake_Wong() {
    this->is_real = false;
    this->meeting = true;
    this->harm_help_times = 3;
  }

  void comeback_to_KamarTaj() {
    this->returned_to_KamarTaj = true;
    this->meeting = false;
    this->harm_help_times = 0;
  }

  bool check_real_Wong() { return (this->meeting && this->is_real); }

  void get_rid_of_fake_Wong() {
    this->meeting = false;
    this->is_real = false;
    this->harm_help_times = 0;
  }
};

struct Wanda {
  bool chance_to_kill;
  bool success_negotiate;
  int doctor_ts;

  Wanda() {
    this->chance_to_kill = false;
    this->success_negotiate = false;
    this->doctor_ts = 0;
  }

  void activate(bool chance_to_kill, bool success_negotiate, int doctor_ts) {
    this->chance_to_kill = chance_to_kill;
    this->success_negotiate = success_negotiate;
    this->doctor_ts = doctor_ts;
  }

  void reset_doctor_TS() { this->doctor_ts = 0; }
};

struct Doors {
  int *doors;
  int size;
  Doors() {
    this->doors = nullptr;
    this->size = 0;
  }

  ~Doors() {
    delete[] this->doors;
    // delete[] this->doors;
    // std::cout << "size: " << this->size << std::endl;
    // for (int i = 0; i < this->size; i++) {
    //   std::cout << "doors[" << i << "]: " << this->doors[i] << std::endl;
    // }
    this->size = 0;
    this->doors = nullptr;
  }

  bool init_doors(std::string str) {
    if (str.size() == 0)
      return false;
    this->doors = new int[str.size()];
    int start = 0;
    for (int i = 0; i < str.size() && start != -1; i++) {
      int tokenized_door = tokenize_door(str, start, " ");
      if (tokenized_door == -1)
        return false;
      this->doors[i] = tokenized_door;
      this->size += 1;
    }
    return true;
  }
};

struct PoisonousMushroom {
  int times;
  bool is_ate;
  int decreased_LV;

  PoisonousMushroom() {
    this->times = 0;
    this->is_ate = false;
    this->decreased_LV = 0;
  }

  void activate_PoisonousMushroom() {
    this->times = 3;
    this->is_ate = true;
  }

  void inactivate_PoisonousMushroom() {
    this->times = 0;
    this->is_ate = false;
    this->decreased_LV = 0;
  }

  void store_decreased_LV(int lv) { this->decreased_LV = lv; }
};

struct TimeThrowback {
  int start;
  bool is_activated;
  int event_th;
  int max_HPs;
  TimeThrowback() {
    this->start = 1;
    this->is_activated = false;
    this->event_th = 0;
    this->max_HPs = MIN_INT;
  }

  void activate() { this->is_activated = true; }

  void store_TimeThrowback(int hp, int start, int event_th) {
    if (hp >= this->max_HPs) {
      this->start = start;
      this->event_th = event_th;
      this->max_HPs = hp;
    }
  }
};

struct Doctor_Strange {
  int MAX_HP;
  int HP;
  int LV;
  int EXP;
  int TS;
  Livitation livitation;
  Wong wong;
  Wanda wanda;
  Doors doors;
  PoisonousMushroom mushroom;
  TimeThrowback time_throwback;

  Doctor_Strange(int hp, int lv, int exp, int ts) {
    this->MAX_HP = hp;
    this->HP = hp;
    this->LV = lv;
    this->EXP = exp;
    this->TS = ts;
  }
};

// Becareful with string.substr();
std::string tokenize(std::string events, int &start, std::string del) {
  int end = (int)events.find(del, start);
  std::string result = "";
  if (end != -1) {
    result = events.substr(start, end - start);
    start = end + 1;
  } else {
    result = events.substr(start, events.size() - start);
    start = events.size();
  }
  return result;
}

bool check_int_code(std::string code) {
  if (code == "")
    return false;
  for (int i = 0; i < code.size(); i++) {
    if (!(code[i] >= '0' && code[i] <= '9')) {
      return false;
    }
  }
  return true;
}

// Becareful with string.substr();
int get_event_code(std::string str, std::string &witchcraft, std::string del) {
  int size = str.size();
  int end = str.find(del);
  std::string result = "";
  if (str[size - 1] != '!') {
    witchcraft = str.substr(end + 1, size);
  } else {
    witchcraft = str.substr(end + 1, size - 1 - (end + 1));
  }
  result = str.substr(0, end);
  if (!check_int_code(result))
    return -1;
  int event_code = stoi(result);
  if (str[size - 1] == '!') {
    if ((1 <= event_code && event_code <= 5) ||
        (event_code >= 7 && event_code <= 9)) {
      if (size != 2)
        return -1;
    } else if (10 <= event_code && event_code <= 11 || event_code == 15) {
      if (size != 3)
        return -1;
    }
  }

  if (str[size - 1] != '!') {
    if ((1 <= event_code && event_code <= 5) ||
        (event_code >= 7 && event_code <= 9)) {
      if (size != 1)
        return -1;
    } else if (10 <= event_code && event_code <= 11 || event_code == 15) {
      if (size != 2)
        return -1;
    }
  }
  return event_code;
}

int get_located_door(std::string str, std::string &s, std::string del) {
  int size = str.size();
  if (size < 3)
    return -1;

  int end = str.find(del);
  std::string result = str.substr(0, end);
  s = str.substr(end + 1, size);
  return check_int_code(result) ? stoi(result) : -1;
}

int tokenize_door(std::string str, int &start, std::string del) {
  int end = (int)str.find(del, start);
  std::string result = "";
  if (end != -1) {
    result = str.substr(start, end - start);
    start = end + 1;
  } else {
    result = str.substr(start, str.size() - start);
    start = -1;
  }
  return check_int_code(result) ? stoi(result) : -1;
}

void calculate_exp_lv_hp_maxhp(Doctor_Strange &doctor_strange, int exp) {
  while (exp > 0) {
    doctor_strange.EXP += exp > 100 ? 100 : exp;
    if (doctor_strange.EXP >= 100) {
      if (doctor_strange.LV < 10) {
        doctor_strange.EXP -= 100;
        doctor_strange.LV += 1;
        doctor_strange.MAX_HP += 50;
        doctor_strange.HP += 10;
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

  for (int i = 2; i <= sqrt(n); i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}

int nearest_hp_prime(int hp) {
  hp++;
  while (!check_prime(hp)) {
    hp++;
  }
  return hp;
}

double long Fibonacci(int n) {
  double long f0 = 0, f1 = 1, fn = 1;

  if (n < 0) {
    return -1;
  } else if (n == 0 || n == 1) {
    return n;
  } else {
    for (int i = 2; i < n; i++) {
      f0 = f1;
      f1 = fn;
      fn = f0 + f1;
    }
  }
  return fn;
}

std::string reverse_string(std::string str) {
  int size = str.size();
  for (int i = 0; i < size / 2; i++) {
    char x = str[i];
    str[i] = str[size - i - 1];
    str[size - i - 1] = x;
  }
  return str;
}

std::string inverted_string(std::string str) {
  int size = str.size();
  if (size <= 2)
    return "DONOTTHING";

  int middle = size / 2;
  std::string left_str = str.substr(0, middle);
  std::string right_str = str.substr(middle + 1, size);
  return reverse_string(left_str) + str[middle] + reverse_string(right_str);
}

bool check_str_code(std::string code) {
  if (code == "")
    return false;
  for (int i = 0; i < code.size(); i++) {
    if (!((code[i] >= 'a' && code[i] <= 'z') ||
          (code[i] >= 'A' && code[i] <= 'Z'))) {
      return false;
    }
  }
  return true;
}

bool displacement_characters_to_ith_and_check(std::string code1,
                                              std::string code2, int ith) {
  if (ith != 0) {
    for (int i = 0; i < code1.size(); i++) {
      if (code1[i] >= 'A' && code1[i] <= 'Z') {
        if ((int)code1[i] + ith > 90) {
          code1[i] = (char)((int)code1[i] + ith - 26);
        } else {
          code1[i] = (char)((int)code1[i] + ith);
        }
      } else {
        if ((int)code1[i] + ith > 122) {
          code1[i] = (char)((int)code1[i] + ith - 26);
        } else {
          code1[i] = (char)((int)code1[i] + ith);
        }
      }
    }
  }
  int index = code1.find(code2);
  return index != -1 ? true : false;
}

bool init_defense_layer(int **&defense_layer, std::string str) {
  defense_layer = new int *[ROW_DEFENSE_LAYER] { nullptr };
  if (str.size() != 2 * ROW_DEFENSE_LAYER * COL_DEFENSE_LAYER - 1)
    return false;
  int i = 0, j = 0;
  int start = 0;
  while (i < ROW_DEFENSE_LAYER && start != str.size()) {
    defense_layer[i] = new int[COL_DEFENSE_LAYER];
    while (j < COL_DEFENSE_LAYER && start != str.size()) {
      std::string s = tokenize(str, start, " ");
      defense_layer[i][j] = stoi(s);
      j++;
    }
    i++;
    j = 0;
  }
  return true;
}

int find_smallest_sum_of_defense_layer(int **&defense_layer, int &index_row,
                                       int &index_col, int m) {
  int cur_sum = 0;
  int result = MIN_INT;
  for (int i = ROW_DEFENSE_LAYER - m; i >= 0; i--) {
    for (int j = COL_DEFENSE_LAYER - m; j >= 0; j--) {
      for (int k = 0; k <= m - 1; k++) {
        for (int h = 0; h <= m - 1; h++) {
          cur_sum += defense_layer[i + k][j + h];
        }
      }

      if (result != MIN_INT) {
        if (cur_sum <= result) {
          result = cur_sum;
          index_row = i;
          index_col = j;
        }
      } else {
        result = cur_sum;
      }
      cur_sum = 0;
    }
  }
  return result;
}

bool check_increase_defense_layer(int **&defense_layer, int index_row,
                                  int index_col, int m) {
  for (int i = index_col; i < index_col + m; i++) {
    for (int j = index_row; j < index_row + m - 1; j++) {
      if (defense_layer[j][i] < defense_layer[j + 1][i])
        return false;
    }
  }
  return true;
}

void delete_defense_layer(int **&defense_layer) {
  for (int i = 0; i < ROW_DEFENSE_LAYER; i++) {
    delete[] defense_layer[i];
  }
  delete[] defense_layer;
  defense_layer = nullptr;
}

bool escape_vitual_space(Doctor_Strange &doctor_strange, int located_doorway,
                         int &num_of_movements) {
  if (located_doorway == MIN_INT)
    return false;
  int max_wanda_movements = log2(doctor_strange.doors.size);
  int left = 0, right = doctor_strange.doors.size - 1;
  while (left < right) {
    num_of_movements++;
    max_wanda_movements--;
    int middle = (left + right) / 2;
    if (doctor_strange.doors.doors[middle] == located_doorway) {
      return true;
    } else if (doctor_strange.doors.doors[middle] < located_doorway) {
      right = middle - 1;
    } else {
      left = middle + 1;
    }
    if (max_wanda_movements == 0) {
      return false;
    }
  }
  return false;
}

bool check_decreased_vitual_space(int *doors, int size) {
  for (int i = 0; i < size - 1; i++) {
    if (doors[i] < doors[i + 1])
      return false;
  }
  return true;
}

int handleEvents(string &HP, string &LV, string &EXP, string &TS,
                 string &events) {
  /// Students have to complete this function and DO NOT modify any parameters
  /// in this function.
  int result = 0;
  // cout << HP << " " << LV << " " << EXP << " " << TS << endl;
  // cout << events << endl;
  int event_size = (int)events.size();

  if (events[0] != '!' || events[event_size - 1] != '!')
    return -1;

  int event_th = 0;
  int start = 1;
  int pre_start = 1;
  int pre_event_th = 0;
  Doctor_Strange doctor_strange(stoi(HP), stoi(LV), stoi(EXP), stoi(TS));
  if (doctor_strange.HP <= 0 || doctor_strange.HP >= 1000)
    return -1;
  if (doctor_strange.LV < 1 || doctor_strange.LV > 10)
    return -1;
  if (doctor_strange.EXP < 0 || doctor_strange.EXP > 100)
    return -1;
  if (doctor_strange.TS < 0 || doctor_strange.TS > 5)
    return -1;

  do {

    // std::cout << "event_th: " << event_th << std::endl;
    // std::cout << "start: " << start << std::endl;
    // std::cout << "HP: " << doctor_strange.HP << std::endl;
    // pre_start = start;
    // pre_event_th = event_th;

    string str = tokenize(events, start, "#");
    if (str[str.size() - 1] == '!')
      str = str.substr(0, str.size() - 1);
    // std::cout << "str: " << str << std::endl;
    if (str == "")
      return -1;
    event_th++;
    pre_start = start;
    pre_event_th = event_th;
    string witchcraft = "";
    int event_code = get_event_code(str, witchcraft, " ");
    if (event_code == -1)
      return -1;

    if (doctor_strange.livitation.has_Livitation &&
        doctor_strange.livitation.resist_damage <= 0) {
      if (doctor_strange.LV < 3) {
        doctor_strange.LV = 1;
      } else {
        doctor_strange.LV -= doctor_strange.livitation.increased_LV;
      }
    }

    // Poisonous mushroom gathering (event 11)
    if (doctor_strange.mushroom.is_ate) {
      if (doctor_strange.mushroom.times == 0) {
        doctor_strange.LV += doctor_strange.mushroom.decreased_LV;
        if (doctor_strange.LV > 10)
          doctor_strange.LV = 10;
      }
      if (doctor_strange.mushroom.times > 0) {
        doctor_strange.mushroom.times--;
      }
    }

    std::cout << "event_code: " << event_code << std::endl;
    // std::cout << "event_th: " << event_th << std::endl;
    // std::cout << "start: " << start << std::endl;
    // std::cout << "witchcraft: " << witchcraft << "@@@" << std::endl;
    switch (event_code) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
      int b = event_th % 10;
      int LV0 = event_th > 6 ? (b > 5 ? b : 5) : b;

      int nearest_prime = nearest_hp_prime(doctor_strange.HP);
      // std::cout << "nearest_prime: " << nearest_prime << std::endl;
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

      // winning
      if (doctor_strange.LV >= LV0 ||
          (doctor_strange.wong.meeting && doctor_strange.wong.is_real)) {
        int exp = 0;
        if (doctor_strange.wong.meeting && doctor_strange.wong.is_real)
          exp = mon_exp;
        else {
          exp = (doctor_strange.LV > LV0) ? mon_exp : mon_exp / 2;
        }

        calculate_exp_lv_hp_maxhp(doctor_strange, exp);
        if ((doctor_strange.wong.meeting && doctor_strange.wong.is_real)) {
          doctor_strange.wong.harm_help_times--;
          // if (doctor_strange.wong.harm_help_times <= 0)
          //   doctor_strange.wong.comeback_to_KamarTaj();
        }

        break;
      }

      // losing
      if (doctor_strange.LV < LV0 ||
          (doctor_strange.wong.meeting && !doctor_strange.wong.is_real)) {
        int damage = 0;
        if (doctor_strange.livitation.check_livitation()) {
          damage = base_damage * LV0 * 10 * (100 - g_function) / 100;
          // std::cout << "g_function: " << g_function << std::endl;
          // std::cout << "damage: " << damage << std::endl;
          // std::cout << "base_damage: " << base_damage << std::endl;
          // std::cout << "LV0: " << LV0 << std::endl;
          doctor_strange.livitation.resist_damage--;
        } else {
          damage = base_damage * LV0 * 10;
        }

        if (doctor_strange.wong.meeting && !doctor_strange.wong.is_real)
          doctor_strange.wong.harm_help_times--;
        doctor_strange.HP -= damage;
      }
      break;
    }
    case 6: {
      if (!check_str_code(witchcraft))
        return -1;
      // Livitation event
      int nearest_prime = nearest_hp_prime(doctor_strange.HP);
      // std::cout << "nearest_prime: " << nearest_prime << std::endl;
      int g_function = (event_th + nearest_prime) % 100;

      int count_attack = 0;
      int count_defence = 0;

      number_of_witchcraft(witchcraft, "attack", 0, 0, count_attack);
      number_of_witchcraft(witchcraft, "defense", 0, 0, count_defence);
      // std::cout << "count_attack: " << count_attack << std::endl;
      // std::cout << "count_defence: " << count_defence << std::endl;

      bool check_livitation = doctor_strange.livitation.check_livitation();
      int blood_loss_reduction_rate =
          check_livitation ? (int)(count_defence * 10 + g_function)
                           : (int)(count_defence * 10);
      int winning_rate = check_livitation
                             ? (int)(count_attack * 10 + g_function)
                             : (int)(count_attack * 10);
      int gargantos_attack = (event_code + witchcraft.size()) % 100;

      // std::cout << "g_function: " << g_function << std::endl;
      // cout << "winning_rate: " << winning_rate << endl;
      // cout << "blood_loss_reduction_rate: " << blood_loss_reduction_rate
      //      << endl;
      // cout << "gargantos_attack: " << gargantos_attack << endl;
      if (winning_rate > gargantos_attack) {
        doctor_strange.TS += 1;
        calculate_exp_lv_hp_maxhp(doctor_strange, 200);
      } else {
        if (blood_loss_reduction_rate < 100) {
          if (check_livitation) {
            doctor_strange.livitation.resist_damage--;
          }
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
      if (!doctor_strange.livitation.is_deprived) {
        if (!doctor_strange.livitation.has_Livitation) {
          doctor_strange.livitation.assign(true, 3, true);
          if (doctor_strange.LV <= 8) {
            doctor_strange.livitation.store_increased_LV(2);
            doctor_strange.LV += 2;
          } else if (doctor_strange.LV == 9) {
            doctor_strange.livitation.store_increased_LV(1);
            doctor_strange.LV += 1;
          }
          // doctor_strange.LV += 2;
          if (!doctor_strange.wong.check_real_Wong())
            doctor_strange.wong.get_rid_of_fake_Wong();
        } else {
          if (!doctor_strange.wong.check_real_Wong()) {
            if (!doctor_strange.livitation.is_real) {
              doctor_strange.LV += doctor_strange.livitation.increased_LV;
              if (doctor_strange.LV > 10) {
                doctor_strange.LV = 10;
              }
            }
            // Recover Livitation function
            doctor_strange.livitation.reset_real_Livitation();
            doctor_strange.wong.get_rid_of_fake_Wong();
          }
        }
      }
      break;
    }
    case 8: {
      // the first time meeting real Wong!
      if (!doctor_strange.wong.meeting &&
          !doctor_strange.wong.returned_to_KamarTaj) {
        doctor_strange.wong.meeting_real_Wong();
        break;
      }

      if (!doctor_strange.wong.meeting && doctor_strange.LV >= 7) {
        doctor_strange.wong.get_rid_of_fake_Wong();
        doctor_strange.livitation.reset_real_Livitation();
        break;
      }

      // meeting fake Wong!
      if ((!doctor_strange.wong.meeting &&
           doctor_strange.wong.returned_to_KamarTaj) ||
          (!doctor_strange.wong.meeting && doctor_strange.LV < 7)) {

        doctor_strange.wong.meeting_fake_Wong();

        // change unreal livitation if doctor_strange has one!
        if (doctor_strange.livitation.has_Livitation) {
          doctor_strange.livitation.is_real = false;
          if (doctor_strange.LV < 3) {
            doctor_strange.LV = 1;
          } else {
            std::cout << "sssssssssssssssssssssssssssss" << std::endl;
            doctor_strange.LV -= doctor_strange.livitation.increased_LV;
          }
          doctor_strange.wong.harm_help_times--;
        }
        break;
      }

      // defeat fake Wong!
      if (doctor_strange.wong.meeting &&
          doctor_strange.wong.harm_help_times > 0)
        doctor_strange.wong.harm_help_times--;

      // if (doctor_strange.wong.harm_help_times <= 0 &&
      //     doctor_strange.wong.check_real_Wong()) {
      //   doctor_strange.wong.comeback_to_KamarTaj();
      // }
      break;
    }
    case 9: {
      doctor_strange.HP = doctor_strange.MAX_HP;
      doctor_strange.wong.get_rid_of_fake_Wong();
      doctor_strange.mushroom.inactivate_PoisonousMushroom();
      doctor_strange.livitation.reset_livitation();
      break;
    }
    case 10: {
      // std::cout << "Fibonacci: " << Fibonacci(doctor_strange.HP - 1)
      //           << std::endl;
      doctor_strange.HP += Fibonacci(doctor_strange.HP - 1);
      break;
    }
    case 11: {
      if (!doctor_strange.wong.check_real_Wong()) {
        doctor_strange.HP -= 50;
        if (!doctor_strange.mushroom.is_ate) {
          doctor_strange.mushroom.activate_PoisonousMushroom();
          doctor_strange.mushroom.store_decreased_LV(doctor_strange.LV);
          doctor_strange.LV = doctor_strange.LV > 3 ? doctor_strange.LV - 2 : 1;
        }
      }
      // real and unreal Wong help and harm Doctor_Strange!
      if (doctor_strange.wong.meeting)
        if (doctor_strange.wong.harm_help_times >= 1)
          doctor_strange.wong.harm_help_times--;
      break;
    }
    case 12: {

      int start_event_12 = 0;
      std::string codes[2]{""};
      // std::cout << "witchcraft: " << witchcraft << std::endl;
      codes[0] = tokenize(witchcraft, start_event_12, " ");
      codes[1] = tokenize(witchcraft, start_event_12, "#");

      // std::cout << "codes0: " << codes[0] << std::endl;
      // std::cout << "codes1: " << codes[1] << std::endl;

      if (!check_str_code(codes[0]) || !check_str_code(codes[1])) {
        return -1;
      }
      int ith = codes[1].size() % 10;
      // std::cout << "ith: " << ith << std::endl;
      if (displacement_characters_to_ith_and_check(inverted_string(codes[0]),
                                                   codes[1], ith)) {

        // Need to check !!!!!!!!!!!!!
        // 1 1 2 3
        // !12 xAbcdez Daf#13 0 2 2 9 5 6 7 1 2 2 1 5 6 7 1 3 3 4 5 6 7 1 3 3 4
        // 5 6 7 1 3 3 4 5 6 7 1 3 3 4 5 6 7 1 3 3 4 5 6 7!
        // std::cout << "HP: "
        // << doctor_strange.HP << std::endl;
        doctor_strange.HP *= 0.9;
        // std::cout << "HP: " << doctor_strange.HP << std::endl;
        // std::cout << "MaxHP: " << doctor_strange.MAX_HP << std::endl;
        doctor_strange.MAX_HP *= 0.9;
        doctor_strange.wanda.activate(false, true, 0);
        calculate_exp_lv_hp_maxhp(doctor_strange, 30);
      } else {
        doctor_strange.wanda.activate(true, false, doctor_strange.TS);
        doctor_strange.livitation.deprived_Livitation();

        if (doctor_strange.LV < 3)
          doctor_strange.LV = 1;
        else
          doctor_strange.LV -= doctor_strange.livitation.increased_LV;

        doctor_strange.TS = 0;
        calculate_exp_lv_hp_maxhp(doctor_strange, 15);
      }
      break;
    }
    case 13: {
      int **defense_layer = nullptr;
      // std::cout << "size: " << witchcraft.size() << std::endl;
      // std::cout << "witchcraft[size-1]: " << witchcraft[witchcraft.size() -
      // 1]
      //           << std::endl;

      if (!init_defense_layer(defense_layer, witchcraft))
        return -1;

      int m = (event_th % 7) > 2 ? (event_th % 7) : 2;
      int index_col, index_row;
      int min_sum = find_smallest_sum_of_defense_layer(defense_layer, index_row,
                                                       index_col, m);
      // std::cout << "min_sum: " << min_sum << std::endl;
      // std::cout << "index_col: " << index_col << std::endl;
      // std::cout << "index_row: " << index_row << std::endl;
      for (int i = 0; i < ROW_DEFENSE_LAYER; i++) {
        for (int j = 0; j < COL_DEFENSE_LAYER; j++) {
          std::cout << defense_layer[i][j] << " ";
        }
        std::cout << std::endl;
      }
      if (!doctor_strange.wong.check_real_Wong()) {
        doctor_strange.wong.get_rid_of_fake_Wong();
      }
      if (check_increase_defense_layer(defense_layer, index_row, index_col,
                                       m)) {
        doctor_strange.HP += (index_col + index_row + 2) * min_sum;
      } else {
        // std::cout << "................" << std::endl;
        doctor_strange.HP -= (index_col + index_row + 2) * min_sum;
        if (doctor_strange.wanda.success_negotiate) {
          if (doctor_strange.HP <= 0)
            doctor_strange.HP = 1;
        }
      }

      delete_defense_layer(defense_layer);
      break;
    }
    case 14: {
      // std::cout << "witchcraft: " << witchcraft << std::endl;
      std::string door_array = "";
      int located_doorway = get_located_door(witchcraft, door_array, " ");
      if (located_doorway == -1)
        return -1;
      // std::cout << "door_array: " << door_array << std::endl;
      // std::cout << "located_doorway: " << located_doorway << std::endl;
      if (!doctor_strange.doors.init_doors(door_array)) {
        return -1;
      }
      int num_of_movements = 0;
      if (!check_decreased_vitual_space(doctor_strange.doors.doors,
                                        doctor_strange.doors.size))
        return -1;

      if (!doctor_strange.wong.check_real_Wong()) {
        doctor_strange.wong.get_rid_of_fake_Wong();
      }

      if (escape_vitual_space(doctor_strange, located_doorway,
                              num_of_movements)) {
        // std::cout << "num_of_movements: " << num_of_movements << std::endl;
        doctor_strange.LV = 1;
        doctor_strange.HP -= num_of_movements * (event_th % 10) * 7;
      } else {
        calculate_exp_lv_hp_maxhp(doctor_strange, 150);
        doctor_strange.livitation.undeprived_Livitation();
        doctor_strange.TS = doctor_strange.wanda.doctor_ts;
        doctor_strange.wanda.reset_doctor_TS();
      }

      // std::cout << "doors: ";
      // for (int i = 0; i < doctor_strange.doors.size; i++) {
      //   std::cout << doctor_strange.doors.doors[i] << " ";
      // }
      //   std::cout << std::endl;
      break;
    }
    case 15: {
      if (!doctor_strange.time_throwback.is_activated &&
          doctor_strange.TS > 0) {
        // std::cout << "case 15 is running!!!" << std::endl;
        if (doctor_strange.HP < doctor_strange.time_throwback.max_HPs) {
          doctor_strange.HP = doctor_strange.time_throwback.max_HPs;
          doctor_strange.LV = 10;
          doctor_strange.EXP = 100;
          start = doctor_strange.time_throwback.start;
          event_th = doctor_strange.time_throwback.event_th;
          doctor_strange.TS--;
        }
        doctor_strange.time_throwback.activate();
      }
      break;
    }
    default: {
      // std::cout << "Default is running!!" << std::endl;
      return -1;
      break;
    }
    }

    // Calculate HP
    if (doctor_strange.HP <= 0) {
      if (doctor_strange.TS > 0) {
        doctor_strange.HP = doctor_strange.MAX_HP;
        doctor_strange.TS--;

        // defeat fake Wong!
        doctor_strange.wong.get_rid_of_fake_Wong();
      } else {
        result = -1;
      }
    } else {
      if (doctor_strange.HP > doctor_strange.MAX_HP) {
        doctor_strange.HP = doctor_strange.MAX_HP;
      }
      if (doctor_strange.MAX_HP > 999) {
        doctor_strange.MAX_HP = 999;
      }
      if (doctor_strange.HP > 999) {
        doctor_strange.HP = 999;
      }
    }

    if (doctor_strange.wong.harm_help_times <= 0) {
      if (doctor_strange.wong.check_real_Wong()) {
        doctor_strange.wong.comeback_to_KamarTaj();
      }
      doctor_strange.wong.harm_help_times = 0;
    }

    // if (doctor_strange.LV >= 7) {
    //   if (!doctor_strange.wong.check_real_Wong()) {
    //     doctor_strange.wong.get_rid_of_fake_Wong();
    //     doctor_strange.livitation.reset_real_Livitation();
    //   }
    // }

    if (!doctor_strange.time_throwback.is_activated)
      doctor_strange.time_throwback.store_TimeThrowback(
          doctor_strange.HP, pre_start, pre_event_th);

    cout << "Doctor: " << doctor_strange.MAX_HP << " " << doctor_strange.HP
         << " " << doctor_strange.LV << " " << doctor_strange.EXP << " "
         << doctor_strange.TS << endl;

    cout << "Livitation: " << doctor_strange.livitation.has_Livitation << " "
         << doctor_strange.livitation.is_real << " "
         << doctor_strange.livitation.resist_damage << " "
         << doctor_strange.livitation.is_deprived << " "
         << doctor_strange.livitation.increased_LV << endl;

    cout << "Wong: " << doctor_strange.wong.harm_help_times << " "
         << doctor_strange.wong.is_real << " " << doctor_strange.wong.meeting
         << " " << doctor_strange.wong.returned_to_KamarTaj << endl;

    std::cout << "Mushroom: " << doctor_strange.mushroom.times << " "
              << doctor_strange.mushroom.is_ate << std::endl;

    std::cout << "Wanda: " << doctor_strange.wanda.chance_to_kill << " "
              << doctor_strange.wanda.success_negotiate << " "
              << doctor_strange.wanda.doctor_ts << std::endl;

    std::cout << "TimeThrowback: " << doctor_strange.time_throwback.max_HPs
              << " " << doctor_strange.time_throwback.event_th << " "
              << doctor_strange.time_throwback.start << " "
              << doctor_strange.time_throwback.is_activated << std::endl;

    std::cout << std::endl;

  } while (start != event_size && result != -1);
  if (result != -1)
    return doctor_strange.TS + doctor_strange.LV + doctor_strange.EXP +
           doctor_strange.HP;
  return result;
}

/// END OF STUDENT'S ANSWER

#endif /* studyInPink_h */
