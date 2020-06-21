#include <iostream>
#include <memory>

class shark
{
public:
    int loc_r;
    int loc_c;
    int speed;
    int heading;
    int size;
    
    shark()
    {
        this->loc_r = 0;
        this->loc_c = 0;
        this->speed = 0;
        this->heading = 0;
        this->size = 0;
    }

    shark(int r, int c, int s, int d, int z)
    {
        this->loc_r = r;
        this->loc_c = c;
        this->speed = s;
        this->heading = d;
        this->size = z;
    }
};

class map
{
public:
    int rows;
    int columns;
    int current_pos;
    std::unique_ptr<std::unique_ptr<shark>[]> map_current;
    std::unique_ptr<std::unique_ptr<shark>[]> map_next;

    map(int R, int C)
    {
        this->rows = R;
        this->columns = C;
        this->current_pos = 0;
        this->map_current = std::make_unique<std::unique_ptr<shark>[]>(R * C);
        this->map_next = nullptr;
    }

    void
    add_shark(shark shk)
    {
        int map_index = (shk.loc_r - 1) * this->columns + (shk.loc_c - 1);
        this->map_current[map_index] = std::make_unique<shark>(shk);
    }

    int
    iterate()
    {
        // STEP 1: fisher moves to the right column
        current_pos++;

        // STEP 2: fisher catches the nearest shark on the column
        int ret = 0;
        for (int i = 0, map_index = this->current_pos - 1; i < this->rows; i++)
        {
            if (this->map_current[map_index] != nullptr)
            {
                ret = this->map_current[map_index]->size;
                this->map_current[map_index].reset(nullptr);
                break;
            }
            map_index += this->columns;
        }

        this->map_next = std::make_unique<std::unique_ptr<shark>[]>(this->rows * this->columns);
        
        // STEP 3: sharks make the move
        // iterate through every shark on the map
        for (int i = 0; i < this->rows * this->columns; i++)
        {
            if (this->map_current[i] == nullptr)
                continue;
            shark *shk = this->map_current[i].get();

            // maximum distance a shark can move on an axis
            int max_movement;
            if (shk->heading <= 2)
                max_movement = this->rows - 1;
            else
                max_movement = this->columns - 1;

            // subtract redundant moves
            int actual_dev = shk->speed % ((max_movement) * 2);
            while (actual_dev > 0)
            {
                int dist;
                if (shk->heading == 1) // heading up
                {
                    dist = std::min((shk->loc_r - 1), actual_dev);
                    shk->loc_r -= dist;
                    if (shk->loc_r == 1)
                        shk->heading = 2;
                }
                else if (shk->heading == 2) // heading down
                {
                    dist = std::min((this->rows - shk->loc_r), actual_dev);
                    shk->loc_r += dist;
                    if (shk->loc_r == this->rows)
                        shk->heading = 1;
                }
                else if (shk->heading == 3) // heading right
                {
                    dist = std::min((this->columns - shk->loc_c), actual_dev);
                    shk->loc_c += dist;
                    if (shk->loc_c == this->columns)
                        shk->heading = 4;
                }
                else // heading left
                {
                    dist = std::min((shk->loc_c - 1), actual_dev);
                    shk->loc_c -= dist;
                    if (shk->loc_c == 1)
                        shk->heading = 3;
                }

                actual_dev -= dist;
            }

            // 'move' the shark to the next map if it managed to survive
            int map_index = (shk->loc_r - 1) * this->columns + (shk->loc_c - 1);
            if (this->map_next[map_index] == nullptr || (this->map_next[map_index]->size < shk->size))
                this->map_next[map_index] = std::move(this->map_current[i]);
        }

        this->map_current = std::move(this->map_next);

        return ret;
    }
};

int
main()
{
    std::ios::sync_with_stdio(false);

    int row, col, sharks;
    std::cin >> row >> col >> sharks;

    std::unique_ptr<map> sharkmap = std::make_unique<map>(row, col);

    int r, c, s, d, z;
    for (int i = 0; i < sharks; i++)
    {
        std::cin >> r >> c >> s >> d >> z;
        sharkmap->add_shark(shark(r, c, s, d, z));
    }

    int ret = 0;
    for (int i = 0; i < col; i++)
    {
        ret += sharkmap->iterate();
    }

    std::cout << ret;

    return 0;
}