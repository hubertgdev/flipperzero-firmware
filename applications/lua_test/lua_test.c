/**
 * This application simulates a 6-sided dice roll.
 */

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>

#include "../../lib/lua-5.4.4/src/lua.h"
#include "../../lib/lua-5.4.4/src/lauxlib.h"
#include "../../lib/lua-5.4.4/src/lualib.h"

#include "../../lib/muffindev/common.h"

/**
 * @brief Defines the type of an event from the device.
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
    UNUSED(ctx);

    canvas_draw_frame(canvas, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    canvas_set_font(canvas, FontPrimary);

    // Write Lua script
    char luaScript[] = "a = 7 + 11";
    // Create Lua context: see it as a virtual machine
    lua_State* L = luaL_newstate();
    // Process the Lua script result
    int result = luaL_dostring(L, luaScript);

    // If the script has been processed successfully
    if(result == LUA_OK) {
        // Get the "a" variable
        lua_getglobal(L, "a");
        // If the variable truly contains a number
        if(lua_isnumber(L, -1)) {
            float aFloat = (float)lua_tonumber(L, -1);
            if(aFloat == 18) {
                canvas_draw_str_aligned(canvas, 0, 0, AlignLeft, AlignBottom, "SUCCESS");
            } else {
                canvas_draw_str_aligned(canvas, 0, 0, AlignLeft, AlignBottom, "FAILURE");
            }
        } else {
            canvas_draw_str_aligned(canvas, 0, 0, AlignLeft, AlignBottom, "NaN");
        }
    } else {
        // Get the Lua stack, and ddisplay it
        //char errormsg[] = lua_tolstring(L, -1, NULL);
        canvas_draw_str_aligned(canvas, 0, 0, AlignLeft, AlignBottom, "ERROR");
    }

    lua_close(L);
}

/**
 * @brief Called when the app received an input.
 */
static void input_callback(InputEvent* input_event, FuriMessageQueue* event_queue) {
    furi_assert(event_queue);

    AppEvent evt = {.type = EventTypeKey, .input = *input_event};
    furi_message_queue_put(event_queue, &evt, FuriWaitForever);
}

/**
 * @brief Entry point method.
 */
int32_t lua_test_app(void* p) {
    UNUSED(p);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(AppEvent));

    // Set render and input callbacks
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Open GUI and register our viewport
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // App loop
    AppEvent evt;
    for(bool processing = true; processing;) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &evt, 100);

        if(event_status == FuriStatusOk) {
            if(evt.input.type == InputTypePress) {
                switch (evt.input.key) {
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
            FURI_LOG_D("Lua_Test", "osMessageQueue: event timeout");
        }
    }

    // End the process
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    return 0;
}
