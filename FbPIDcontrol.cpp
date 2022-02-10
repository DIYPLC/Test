#include "FbPIDcontrol.hpp"

void FbPIDcontrol::execute(void)
{
  //Временные переменные, не сохраняемые.
  float Er    = 0.0; //Ошибка регулирования.
  float Ppart = 0.0; //Пропорциональная составляющая регулятора.
  float Dpart = 0.0; //Дифференциальная составляющая регулятора.
  float Auto  = 0.0; //Выход ПИД- алгоритма и вход переключателя режимов работы.
  float Sw    = 0.0; //Выход с переключателя режимов работы.

  //Ошибка регулирования.
  Er = SP - PV;

  //Зона нечувствительности к ошибке.
  if ((DBmin < Er) && (Er < DBmax))
  {
    Er = 0.0;
  }

  //Пропорциональная составляющая.
  Ppart = Kp * Er;

  //Интегральная составляющая.
  if (Ki == 0.0)
  {
    Ipart = 0.0;
  }
  else
  {
    Ipart = Ipart + Ki * Er * Ts;
  }

  //Дифференциальная составляющая.
  if (Kd == 0.0)
  {
    Dpart = 0.0;
    Dintegral = 0.0;
  }
  else
  {
    Dpart = (Er * Kd - Dintegral) * Kdf;
    Dintegral = Dintegral + (Ts * Dpart);
  }

  //Сигнал управления.
  Auto = Ppart + Ipart + Dpart;

  //Переключение режима работы "Ручной / Автоматический".
  if (ManOn)
  {
    Sw = Manual;
  }
  else
  {
    Sw = Auto;
  }

  //Амплитудный ограничитель- максимум.
  if (Sw >= OPmax)
  {
    OP = OPmax;
    Ipart = OP - (Ppart + Dpart);
  }
  //Амплитудный ограничитель- минимум.
  if (Sw <= OPmin)
  {
    OP = OPmin;
    Ipart = OP - (Ppart + Dpart);
  }
  //Амплитудный ограничитель- без ограничений.
  if ((OPmin < Sw) && (Sw < OPmax))
  {
    OP = Sw;
    if (ManOn)
    {
      Ipart = OP - (Ppart + Dpart);
    }
  }
  return;
}

// Передаточная функция регулятора (при Ts->0) :
//         OP(s)             1             s
// W(s) = ------- = Kp + Ki*--- + Kd*-------------
//         Er(s)             s        Tdf * s + 1
//
// Er(s) = SP(s) - PV(s).
// Tdf = 1 / Kdf

// Передаточная функция интегратора:
//         Z * Ts
// W(z) = --------
//         Z - 1

// Численное интегрирование методом прямоугольников.
// y = y + x * Ts;

// Перечень сокращений.
// ПИД- регулятор- пропорционально интегрально дифференциальный регулятор
// PV- Process Variable.
// SP- Set Point.
// Er- Error.
// DBmax- Dead band maximum.
// DBmin- Dead band minimum.
// OPmax- Output power maximum.
// OPmin- Output power minimum.
// Ts- Sample Time.
// ManOn- Manual on.
// OP- Output power.
// Sw- Switch.
// Fb- Functional block.
// Db- Data block.

//      +---+
//      |   |
// SP->-|+  |
//      |   |->-Er
// PV->-|-  |
//      |   |
//      +---+

//          DBmax
//      +-----------+
//      |           |
//      |        /  |
//      |       /   |
//      |      |    |
// Er->-|   +--+    |->-Er
//      |   |       |
//      |  /        |
//      | /         |
//      |           |
//      +-----------+
//          DBmin

//      +---+
//      |   |
// Er->-|   |
//      | * |->-Ppart
// Kp->-|   |
//      |   |
//      +---+

//      +---+   +----------+
//      |   |   |          |
// Er->-|   |   |  Z * Ts  |
//      | * |->-| -------- |->-Ipart
// Kp->-|   |   |  Z - 1   |
//      |   |   |          |
//      +---+   +----------+

//                        +---+
//                        |   |
//      +---+       Kdf->-|   |
//      |   |     +---+   |   |
// Er->-|   |     |   |   | * |----------------+--->-Dpart
//      | * |--->-|+  |   |   |                |
// Kd->-|   |     |   |->-|   |                |
//      |   | +->-|-  |   |   | +----------+   |
//      +---+ |   |   |   +---+ |          |   |
//            |   +---+         |  Z * Ts  |   |
//            +-----------------| -------- |-<-+
//                              |  Z - 1   |
//                              |          |
//                              +----------+

//         +---+
//         |   |
// Ppart->-|+  |
//         |   |
// Ipart->-|+  |->-Auto
//         |   |
// Dpart->-|+  |
//         |   |
//         +---+

//          +-----+
//          |     |
// Auto--->-|-+   |
//          |  \  |
//          |   +-|->-Sw
//          |     |
// Manual->-|-+   |
//          |     |
//          +-----+
//             |
// ManOn-->----+

//          OPmax
//      +-----------+
//      |           |
//      |       +-- |
//      |      /    |
// Sw->-|     /     |->-OP
//      |    /      |
//      | --+       |
//      |           |
//      +-----------+
//          OPmin

//  +---------+
//  | GNU GPL |
//  +---------+
//  |
//  |
//  .= .-_-. =.
// ((_/)o o(\_))
//  `-'(. .)`-'
//  |/| \_/ |\
//  ( |     | )
//  /"\_____/"\
//  \__)   (__/
