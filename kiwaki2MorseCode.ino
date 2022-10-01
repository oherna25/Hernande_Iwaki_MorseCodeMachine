/*
 * Kenshiro Iwaki - 664864905
 * Project - Morse Code Machine
 * Description: Create a morse code and display the translated message on the 16x2 display. Tap the left button for "dot", long press for "dash". Press the right button to reset.
 *              Waiting at least 1 second after the last left button press will add a space to the message. Long pressing the left button for at least 1 second will stop the message.
 * Assumptions: I can use a similar circuit for lab8 for the 16x2 display and the buttons.
 * References: https://medium.com/swlh/how-tree-data-structures-help-us-understand-morse-code-a95f6f7f2219
 *             https://www.boxentriq.com/code-breaking/morse-code
 *             https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/
 */

#include <LiquidCrystal.h>

struct Node {
  char c;
  Node *left;  // dot
  Node *right;  // dash
};

class MorseCode {
private:
  Node *root;
  Node *curr;  // node for the current character
  String message;
  int len;  // length of the message

public:
  MorseCode();  // set up the tree
  void dot();  // add dot to current character
  void dash();  // add dash to current character
  void addSpace();  // add a space to the current message
  String getMessage() const;  // get the entire message
  int getLength() const;  // get the length of the message
  void reset();  // reset to empty message
};

MorseCode::MorseCode() {
  Node *C0 = new Node{'0', nullptr, nullptr};
  Node *C1 = new Node{'1', nullptr, nullptr};
  Node *C2 = new Node{'2', nullptr, nullptr};
  Node *C3 = new Node{'3', nullptr, nullptr};
  Node *C4 = new Node{'4', nullptr, nullptr};
  Node *C5 = new Node{'5', nullptr, nullptr};
  Node *C6 = new Node{'6', nullptr, nullptr};
  Node *C7 = new Node{'7', nullptr, nullptr};
  Node *C8 = new Node{'8', nullptr, nullptr};
  Node *C9 = new Node{'9', nullptr, nullptr};
  Node *plus = new Node{'+', nullptr, nullptr};
  Node *equal = new Node{'=', nullptr, nullptr};
  Node *slash = new Node{'/', nullptr, nullptr};
    
  Node *H = new Node{'H', C5, C4};
  Node *V = new Node{'V', nullptr, C3};
  Node *F = new Node{'F', nullptr, nullptr};
  Node *unknown1 = new Node{' ' , nullptr, C2};
  Node *L = new Node{'L', nullptr, nullptr};
  Node *unknown2 = new Node{' ', plus, nullptr};
  Node *P = new Node{'P', nullptr, nullptr};
  Node *J = new Node{'J', nullptr, C1};
  Node *B = new Node{'B', C6, equal};
  Node *X = new Node{'X', slash, nullptr};
  Node *C = new Node{'C', nullptr, nullptr};
  Node *Y = new Node{'Y', nullptr, nullptr};
  Node *Z = new Node{'Z', C7, nullptr};
  Node *Q = new Node{'Q', nullptr, nullptr};
  Node *unknown3 = new Node{' ', C8, nullptr};
  Node *unknown4 = new Node{' ', C9, C0};

  Node *S = new Node{'S', H, V};
  Node *U = new Node{'U', F, unknown1};
  Node *R = new Node{'R', L, unknown2};
  Node *W = new Node{'W', P, J};
  Node *D = new Node{'D', B, X};
  Node *K = new Node{'K', C, Y};
  Node *G = new Node{'G', Z, Q};
  Node *O = new Node{'O', unknown3, unknown4};

  Node *I = new Node{'I', S, U};
  Node *A = new Node{'A', R, W};
  Node *N = new Node{'N', D, K};
  Node *M = new Node{'M', G, O};
  Node *E = new Node{'E', I, A};
  Node *T = new Node{'T', N, M};

  root = new Node{' ', E, T};
  curr = root;
  message = " ";
  len = 1;
}

void MorseCode::dot() {
  if (curr->left != nullptr) {
    curr = curr->left;
  }
  message[len - 1] = curr->c;  // update the last character
}

void MorseCode::dash() {
  if (curr->right != nullptr) {
    curr = curr->right;
  }
  message[len - 1] = curr->c;  // update the last character
}

void MorseCode::addSpace() {
  message += " ";
  len++;
  curr = root;  // reset the current character
}

String MorseCode::getMessage() const {
  return message;
}

int MorseCode::getLength() const {
  return len;
}

void MorseCode::reset() {
  curr = root;
  message = " ";
  len = 1;
}

// global variables
const int rs{12}, en{11}, d4{5}, d5{4}, d6{3}, d7{2};
const int btn1{6}, btn2{7};
int btn1State{}, btn2State{};
int counter1{}, counter2{};
bool finished{false};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
MorseCode mc;

void setup() {
  lcd.begin(16, 2);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
}

void loop() {
  // read the state of the button press
  btn1State = digitalRead(btn1);
  btn2State = digitalRead(btn2);

  if (btn1State == HIGH) {
    counter1++;
    counter2 = 0;
  } else {
    if (counter1 < 15 && counter1 > 0 && !finished) {
      mc.dot();
    } else if (counter1 >= 15 && counter1 < 100 && !finished) {
      mc.dash();
    } else if (counter1 >= 100) {
      finished = true;  // message is complete
    }
    counter1 = 0;
    counter2++;
  }

  // add a space to message after at least 1 second after last button press (10 * 100 = 1000 millisecond = 1 second)
  if (counter2 >= 100) {
    if (mc.getMessage() != " " && !finished) {
      mc.addSpace();
    }
    counter2 = 0;
  }

  // reset the message
  if (btn2State == HIGH) {
    mc.reset(); 
    counter2 = 0;
    finished = false;
  }
  
  lcd.setCursor(0, 0);
  lcd.print(mc.getMessage());
  if (finished) {
    lcd.setCursor(0, 1);
    lcd.print("Done");
  }
  delay(10);
  lcd.clear();
}
