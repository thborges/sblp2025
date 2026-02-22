/*
 * Robcmp examples: Breakout game on a SSD1306 display
 */

#include "intfs/mcu.hpp"
#include "intfs/display.hpp"
#include "graphic/canvas8.hpp"
#include "math.hpp"

template<buffer8 bufferimpl, display displayimpl, mcu mcuimpl, databus uartimpl>
class game {
private:
    canvas8<bufferimpl>& canvas;
    displayimpl& display;
    mcuimpl& mmcu;
    uartimpl& uuart;

    // Bar related vars
    uint8_t bar_middle = 0u;
    uint8_t bar_width = 30u;
    int8_t bar_next_move = 0;

    // Ball related vars
    signed char ball_dir_x = 1;
    signed char ball_dir_y = 1;
    uint8_t ball_pos_x = 0u;
    uint8_t ball_pos_y = 0u;

    // Game status
    bool paused = false;
    bool gameover = false;
    uint8_t level = 0;
    uint8_t rem_blocks = 0;

    // Blocks const and vars
    uint8_t start_y = 0u;
    const uint8_t bottom_margin = 4;

    /* The size and position of game elements
       blocks = 129/(width+1)
       use block width = 5, 21 blocks */
    const uint8_t block_size = 4;
    const uint8_t block_sizesp = 4+1;
    const uint8_t right_margin = 25 * block_sizesp - 1;

    const uint8_t blocks_rows = 5;
    const uint8_t blocks_cols = 25;
    bool blocks[5][25] = {};

public:
    game(canvas8<bufferimpl>& canvas, displayimpl& display, mcuimpl& mmcu, uartimpl& uart) :
        canvas(canvas), display(display), mmcu(mmcu), uuart(uart) {
        for(uint8_t r = 0; r < blocks_rows; r++)
            for(uint8_t c = 0; c < blocks_cols; c++)
                blocks[r][c] = true;            
    }

    void process_game() {
        check_level_done();

        move_bar();

        if (!paused && !gameover) {
            //mmcu.wait_ms(20);
            move_ball();
        }
    }

    void move_bar() {
        if (bar_next_move == 0) {
            return;
        }
        auto pixels = bar_next_move;
        bar_next_move = 0;

        auto left_limit = bar_width/2 - 1;
        auto right_limit = right_margin - bar_width/2;
        
        if (bar_middle + pixels < left_limit) {
            pixels = left_limit - bar_middle + 1;
        } else if (bar_middle + pixels > right_limit) {
            pixels = right_limit - bar_middle;
        }
    
        if (pixels == 0) {
            return;
        }
        
        auto move = abs8(pixels);
        bar_middle += pixels;
        auto start = bar_middle - bar_width/2;
        auto end = start + bar_width - 1;
        auto set = pixels > 0;
        if (pixels < 0) {
            canvas.fill_rect(start, bottom_margin-4, start+move-1, bottom_margin-2, true);
            canvas.fill_rect(end+1, bottom_margin-4, end+move, bottom_margin-2, false);
        } else {
            canvas.fill_rect(start-move, bottom_margin-4, start-1, bottom_margin-2, false);
            canvas.fill_rect(end-move+1, bottom_margin-4, end, bottom_margin-2, true);
        }

        if (paused) {
            // when paused, set ball direction according
            // to the first movement
            if (pixels > 0) {
                ball_dir_x = 1;
            } else {
                ball_dir_x = -1;
            }
        }
    }

    void update_block(uint8_t row, uint8_t col) {
        auto c = col * (block_size+1);
        auto r = start_y + row * (block_size+1);
        auto status = blocks[row][col];
        canvas.fill_rect(c, r, c+block_size-1, r+block_size-1, status);
    }
    
    void draw_blocks() {
        // update blocks according to the level
        if (level > 1) {
            uint8_t i = 0u;
            uint8_t total = blocks_rows * blocks_cols;
            const uint8_t step = 37u;

            while (i < level*5) {
                uint16_t idx = (level + i * step) % total;
                uint8_t arow = idx / blocks_cols;
                uint8_t acol = idx % blocks_cols;
                blocks[arow][acol] = true;
                i++;
            }
        }

        // draw blocks
        uint8_t row = 0u;
        while (row < blocks_rows) {
            uint8_t col = 0u;
            while (col < blocks_cols) {
                if (level > 1) {
                    bool v = !blocks[row][col];
                    blocks[row][col] = v;
                }
                update_block(row, col);
                if (blocks[row][col]) {
                    rem_blocks++;
                }
                col++;
            }
            row++;
            //mmcu.wait_ms(100); // animation effect
            //display.update_frame();
        }
        //display.update_frame();
    }
    
    void draw_game_uart(uint8_t brow, uint8_t bcol) {
        uuart.write_array((char*)"\nrow: ", 6);
        uuart.write_u16(brow);
        uuart.write_array((char*)" col: ", 6);
        uuart.write_u16(bcol);
        uuart.write('\n');

        uint8_t row = 0u;
        while (row < blocks_rows) {
            uint8_t col = 0u;
            while (col < blocks_cols) {
                if (blocks[row][col]) {
                    uuart.write('.');
                } else {
                    uuart.write(' ');
                }
                col++;
            }
            row++;
            uuart.write('\n');
        }
    }

    void init_level() {
        start_y = canvas.height() - blocks_rows * block_sizesp;
    
        draw_blocks();
    
        // hide current bar
        uint8_t start = bar_middle - bar_width/2;
        canvas.fill_rect(start, bottom_margin-4, start + bar_width-1, bottom_margin-2, false);
        bar_width = 30 - level;
        if (bar_width < 10) {
            bar_width = 10;
        }
        // draw new bar
        bar_middle = uint8_t(right_margin/2);
        start = bar_middle - bar_width/2;
        canvas.fill_rect(start, bottom_margin-4, start + bar_width-1, bottom_margin-2, true);
    
        // set ball initial pos
        ball_dir_x = 1;
        ball_dir_y = 1;
        ball_pos_x = bar_middle;
        ball_pos_y = bottom_margin;
        canvas.fill_rect(ball_pos_x - 1, ball_pos_y - 1, ball_pos_x, ball_pos_y, true);
    }
    
    void move_ball() {
        canvas.fill_rect(ball_pos_x - 1, ball_pos_y - 1, ball_pos_x, ball_pos_y, false);
    
        // ball_pos_{x,y} represents the top right pixel of the ball
    
        if (ball_pos_y >= start_y - 1) {
            uint8_t ball_edge_x = ball_pos_x;
            if (ball_dir_x == -1) {
                ball_edge_x--;
            }
            uint8_t bcol = ball_edge_x / block_sizesp;
    
            uint8_t brow = (ball_pos_y + 1 - start_y) / block_sizesp;
            if (ball_dir_y == -1 && ball_pos_y >= start_y) {
                brow = (ball_pos_y - start_y) / block_sizesp;
            }
            
            if (brow < blocks_rows and 
                bcol < blocks_cols and 
                blocks[brow][bcol]) {

                blocks[brow][bcol] = false;
                rem_blocks--;
                update_block(brow, bcol);
                //draw_game_uart(brow, bcol);

                auto hit_y = (ball_pos_y + 1 - start_y) % block_sizesp == 0;
                auto hit_x = ball_pos_x % block_sizesp == 0;

                if (hit_y) {
                    ball_dir_y *= -1;
                } else if (hit_x) {
                    ball_dir_x *= -1;
                }
            }
        }
    
        if ((ball_dir_x == -1 and ball_pos_x - 1 == 0) || // hit left margin
            (ball_dir_x == 1 and ball_pos_x + 1 == right_margin)) { // hit right margin
            ball_dir_x *= -1;
        }
    
        if (ball_dir_y == 1 && ball_pos_y == canvas.height() - 1) {
            // hit top
            ball_dir_y *= -1;
    
        } else if (ball_dir_y == -1 && ball_pos_y - 1 == bottom_margin) {
            // hit bottom
            ball_dir_y *= -1;
            auto mwidth = bar_width/2;
            if (ball_pos_x < bar_middle - mwidth ||
                ball_pos_x > bar_middle + mwidth) {
                //gameover = true;
            }    
        }
    
        ball_pos_x += ball_dir_x;
        ball_pos_y += ball_dir_y;
    
        if (rem_blocks > 0) {
            canvas.fill_rect(ball_pos_x - 1, ball_pos_y - 1, ball_pos_x, ball_pos_y, true);
        }
    }

    void check_level_done() {
        if (rem_blocks == 0) {
            //set_level_debug();
            level = (level+1) % 30;
            init_level();
            //draw_game_uart(0, 0);
            //paused = true;
        }
    }

    uint8_t get_bar_width() {
        return bar_width;
    }
    
    /*key_press implements interrupt_i8 {
        void handler(uint8 key) {
            if gameover {
                gameover = false;
                level = 0;
            } else {
                if key == 'C' {// right
                    bar_next_move += 10;
                } else if key == 'D' {
                    bar_next_move -= 10;
                }
            }
            paused = false;
        }
    }*/

};
