/**
 * This application simulates a 6-sided dice roll.
 */

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>

const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t MIN_DICE_VALUE = 1;
const uint8_t MAX_DICE_VALUE = 6;
const uint8_t DICE_DOT_RADIUS = 4;
const uint8_t DICE_DOT_SPACE = 6;

/**
 * @brief Represents the current state of the app.
 */
typedef struct {
    uint8_t last_output;
} DiceState;

/**
 * @brief Defines the type of an event from the device
 */
typedef enum {
    EventTypeTick,
    EventTypeKey,
} AppEventType;

/**
 * @brief Represents an event from the device.
 */
typedef struct {
    AppEventType type;
    InputEvent input;
} AppEvent;

/**
 * @brief Called every frame.
 * Renders the GUI of the app.
 */
static void render_callback(Canvas* const canvas, void* ctx) {
    const DiceState* dice_state = acquire_mutex((ValueMutex*)ctx, 25);
    if(dice_state == NULL) {
        return;
    }

    canvas_set_font(canvas, FontPrimary);

    uint8_t center_x    = SCREEN_WIDTH / 2;
    uint8_t center_y    = SCREEN_HEIGHT / 2;
    uint8_t left        = center_x - DICE_DOT_RADIUS - DICE_DOT_SPACE;
    uint8_t right       = center_x + DICE_DOT_RADIUS + DICE_DOT_SPACE;
    uint8_t top         = center_y - DICE_DOT_RADIUS - DICE_DOT_SPACE;
    uint8_t bottom      = center_y + DICE_DOT_RADIUS + DICE_DOT_SPACE;
    uint8_t area_size   = DICE_DOT_SPACE * 5 + DICE_DOT_RADIUS * 3;

    canvas_draw_frame(
        canvas,
        center_x - area_size / 2 - 1,
        center_y - area_size / 2 - 1,
        area_size,
        area_size
    );
    
    if(dice_state->last_output == 1) {
        // Center dot
        canvas_draw_disc(canvas, center_x - DICE_DOT_RADIUS / 2, center_y - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else if (dice_state->last_output == 2) {
        // Top-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-right
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else if (dice_state->last_output == 3) {
        // Top-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Center dot
        canvas_draw_disc(canvas, center_x - DICE_DOT_RADIUS / 2, center_y - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-right
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else if (dice_state->last_output == 4) {
        // Top-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Top-right dot
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-left
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-right
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else if (dice_state->last_output == 5) {
        // Top-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Top-right dot
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Center dot
        canvas_draw_disc(canvas, center_x - DICE_DOT_RADIUS / 2, center_y - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-left
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-right
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else if (dice_state->last_output == 6) {
        // Top-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Top-right dot
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, top - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Middle-left dot
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_SPACE - DICE_DOT_RADIUS / 2 - DICE_DOT_RADIUS, DICE_DOT_RADIUS);
        // Middle-right dot
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_SPACE - DICE_DOT_RADIUS / 2 - DICE_DOT_RADIUS, DICE_DOT_RADIUS);
        // Bottom-left
        canvas_draw_disc(canvas, left - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
        // Bottom-right
        canvas_draw_disc(canvas, right - DICE_DOT_RADIUS / 2, bottom - DICE_DOT_RADIUS / 2, DICE_DOT_RADIUS);
    }
    else {
        canvas_draw_str_aligned(canvas, center_x - 2, center_y - 4, AlignRight, AlignBottom, "?");
    }

    release_mutex((ValueMutex*)ctx, dice_state);
}

/**
 * @brief Called when the app received an input.
 */
static void input_callback(InputEvent* input_event, osMessageQueueId_t event_queue) {
    furi_assert(event_queue);

    AppEvent evt = {.type = EventTypeKey, .input = *input_event};
    osMessageQueuePut(event_queue, &evt, 0, osWaitForever);
}

/**
 * @brief Generates a random number between given minimum (inclusive) and maximum (exclusive) values.
 */
static uint8_t generate_random_number(const uint8_t min, const uint8_t max) {
    return rand() % (max - min) + min;
}

/**
 * @brief Entry point method.
 */
int32_t dice_roller_app(void* p) {
    UNUSED(p);
    srand(DWT->CYCCNT);

    osMessageQueueId_t event_queue = osMessageQueueNew(8, sizeof(AppEvent), NULL);

    // Setup the plugin state
    DiceState* dice_state = malloc(sizeof(DiceState));
    dice_state->last_output = 1;

    // ValueMutex is a wrapper to use native mutex and value pointer. Here, it's used to make the plugin state synchronized between threads
    ValueMutex state_mutex;
    if(!init_mutex(&state_mutex, dice_state, sizeof(DiceState))) {
        FURI_LOG_E("Hello_World", "Cannot create mutex\r\n");
        free(dice_state);
        return 255;
    }

    // Set render and input callbacks
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Open GUI and register our viewport
    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // App loop
    AppEvent evt;
    for(bool processing = true; processing;) {
        osStatus_t event_status = osMessageQueueGet(event_queue, &evt, NULL, 100);
        DiceState* dice_state = (DiceState*)acquire_mutex_block(&state_mutex);

        if(event_status == osOK) {
            if(evt.input.type == InputTypePress) {
                switch (evt.input.key) {
                    case InputKeyOk: {
                        dice_state->last_output = generate_random_number(MIN_DICE_VALUE, MAX_DICE_VALUE + 1);
                        view_port_update(view_port);
                    } break;
                    case InputKeyBack: {
                        processing = false;
                        view_port_update(view_port);
                    }
                        break;
                    default:
                        break;
                }
            }
        }
        else {
            FURI_LOG_D("Dice_Roller", "osMessageQueue: event timeout");
        }

        // Release the plugin state mutex block to unlock the state object
        release_mutex(&state_mutex, dice_state);
    }

    // End the process
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close("gui");
    view_port_free(view_port);
    osMessageQueueDelete(event_queue);

    return 0;
}
