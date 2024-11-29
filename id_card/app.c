#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>

#include "id_card_icons.h"

#define NAME   "Petros Trak"
#define NUMBER "+30 6906622975"
#define EMAIL  "pit.trak@gmail.com"

#define TAG "Id card"

// The goal of this callback is to draw the GUI of your application.
// It will be called each time the GUI needs to be refreshed.
//
// canvas: An internal variable that you will use to draw on the GUI.
// context: A variable which you can define and use to get some data in your callbacks.
void draw_callback(Canvas* canvas, void* context);

// The input callback puts the new InputEvent into the message queue.
// This event will be handled by the main.
void input_callback(InputEvent* event, void* context);

typedef struct {
    FuriMessageQueue* input_queue;
    ViewPort* view_port;
    Gui* gui;
} Id_card;

int32_t id_card_main(void* p) {
    UNUSED(p);
    Id_card app;

    // Allocation.
    app.view_port = view_port_alloc();
    app.input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Callbacks.
    view_port_draw_callback_set(app.view_port, draw_callback, &app);
    view_port_input_callback_set(app.view_port, input_callback, &app);

    // GUI.
    app.gui = furi_record_open("gui");
    gui_add_view_port(app.gui, app.view_port, GuiLayerFullscreen);

    // Input handling.
    InputEvent input;
    uint8_t exit_loop = 0;

    FURI_LOG_I(TAG, "Start the main loop.");
    while(1) {
        // We check if there is an error.
        furi_check(
            // We wait for the next user input and store it in the input variable.
            furi_message_queue_get(app.input_queue, &input, FuriWaitForever) == FuriStatusOk);

        switch(input.key) {
        case InputKeyLeft:
        case InputKeyRight:
        case InputKeyOk:
        case InputKeyUp:
        case InputKeyDown:
        case InputKeyBack:
            exit_loop = 1;
            break;
        default:
            break;
        }

        if(exit_loop) {
            break;
        }

        view_port_update(app.view_port);
    }

    // Free all structures.
    view_port_enabled_set(app.view_port, false);
    gui_remove_view_port(app.gui, app.view_port);
    furi_record_close("gui");
    view_port_free(app.view_port);

    return 0;
}

void draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    canvas_draw_str_aligned(canvas, 1, 5, AlignLeft, AlignTop, "Name: ");

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str_aligned(canvas, 33, 5, AlignLeft, AlignTop, "Name: ");

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str_aligned(canvas, 33, 5, AlignLeft, AlignTop, NAME);

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    canvas_draw_str_aligned(canvas, 1, 20, AlignLeft, AlignTop, "N: ");

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str_aligned(canvas, 15, 20, AlignLeft, AlignTop, NUMBER);

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    canvas_draw_str_aligned(canvas, 1, 35, AlignLeft, AlignTop, "EMAIL: ");

    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str_aligned(canvas, 5, 45, AlignLeft, AlignTop, EMAIL);

    canvas_draw_icon(canvas, 95, 5, &I_icon_30x30);
}

void input_callback(InputEvent* event, void* context) {
    Id_card* app = context;
    furi_message_queue_put(app->input_queue, event, 0);
}
