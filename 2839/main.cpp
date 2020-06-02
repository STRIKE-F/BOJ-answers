#include <iostream>

using namespace std;

int main()
{
    int weight;
    int three = 0, five = 0;
    cin >> weight;
    
    five = weight / 5;
    while (true)
    {
        if ((weight - 5 * five) % 3 == 0)
        {
            three = (weight - 5 * five) / 3;
            break;
        }
        else
        {
            five--;
            if (five < 0)
            {
                cout << -1;
                return 0;
            }
        }
    }
    
    cout << five + three;

    return 0;
}

