#include <iostream>
#include <vector>

using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::vector;


enum suit { CLUBS, DIAMONDS, SPADES, HEARTS };
enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};


class Card{
private:
    suit m_Suit;
    rank m_Rank;
    bool m_IsFaceUp;
public:
    Card(suit s, rank r, bool p): m_Suit(s),m_Rank(r), m_IsFaceUp(p){}
    void Flip(){m_IsFaceUp = !m_IsFaceUp;}
    int getValue() const {
	    return (m_Rank > 9) ? 10 : m_Rank;
	}
};

// Task3
class Hand{
private:
	vector<Card*> m_Cards;
public:
	void Add(Card* pCard){m_Cards.push_back(pCard);}
	void Clear(){m_Cards.clear();}
	int GetValue(){
		int sum = 0;
		vector<Card*>::const_iterator it;
		it = m_Cards.begin();
		while(it != m_Cards.end()){
			if(((*it)->getValue())==1)
				sum+=(sum+11>21)? 1 : 11;
			else
				sum+=(*it)->getValue();
			++it;
		}
		return sum;
	}
};

int main(int argc,char** args){

    cout << "\t\tWelcome to Blackjack!\n\n";
    
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        cout << "How many players? (1 - 7): ";
        cin >> numPlayers;
    }
    
    vector<string> names;
    string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        cout << "Enter player name: ";
        cin >> name;
        names.push_back(name);
    }
    cout << endl;
    
    // игровой цикл
/*    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> again;
    }
  */  

    Hand hand;  
	Card card1(SPADES,EIGHT,true);
	hand.Add(&card1);
	Card card2(SPADES,THREE,true);
	hand.Add(&card2);
	Card card3(SPADES,ACE,true);
	hand.Add(&card3);
	
    cout << "Points: " << hand.GetValue() << endl;
	hand.Clear();
    cout << "Points: " << hand.GetValue() << endl;

    return 0;
}
