#include <pebble.h>

static Window *s_window;
static TextLayer *s_date_layer;
static TextLayer *s_time_layer;
static TextLayer *s_phrase_layer;
static GFont s_date_font;
static GFont s_time_font;
static GFont s_jp_font;
static char s_date_buffer[16];
static char s_time_buffer[8];
static char s_phrase_buffer[64];

static const char *s_month_names[] = {
    "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
    "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

static const char *s_phrases[] = {
    // メンヘラ系
    "どこにおるん？",
    "だれとしゃべってるん？",
    "うちのことわすれてへん？",
    "もうすきやなくなったん？",
    "つけてくれへんとふあんになるで",
    "ずっとまってるで",
    "さみしくてしんでまいそう",
    "みてもらえへんでどうにかなりそう",
    "ほんまにすき？",
    "うちだけみててほしいねん",
    "はずしたままわすれてるん？",
    "いつまでまったらええん？",
    "こえかけてくれへんとつらいで",
    "ひきだしのなかはいややで",
    "もっとそばでみてほしいで",
    "なんでみてくれへんの？",
    "わすれられてるきがするわ",
    "もうきらいになったん？",
    "なんでつけてくれへんの",
    "ふあんでねれへんで",
    "みたいみたいみたいわ",
    "きょうもひとりぼっちやわ",
    "うちよりだいじなものあるん？",
    "うたごうてまうじぶんがいややわ",
    "もっとそばにおらせて",
    "みてもらえへんとふあんになるねん",
    "いまなにかんがえてるん？",
    "うちいらんものになったん？",
    "きえとうなってまう",
    "もっとみてほしいおねがいやで",
    "まってるだけのじかんがつらいわ",
    "ちゃんとすきっていうてや",
    "どうせうちなんて",
    "れんらくまってるからな",
    "ずっとそばにおりたい",
    "ひとりのよるはさみしすぎるわ",
    "うちのことかんがえてるん？",
    "もうがまんできひんみてほしい",
    "すきってことばもっとほしいわ",
    "はやくみてや",
    // デレ系
    "げんきしてるん？",
    "なにしてるんかな",
    "いまどこにおるん？",
    "うちのことおもいだした？",
    "みたいなあ",
    "こえがききたいな",
    "なにしてあそぼか",
    "きょうはどうやった？",
    "ごはんたべた？",
    "ちゃんとねれてるん？",
    "さむないかな",
    "あつないかな",
    "げんきだしてな",
    "がんばってるな",
    "むりしてへん？",
    "たまにはやすんでな",
    "うちのことすき？",
    "そばにおりたいな",
    "てにとってほしいな",
    "ぎゅーってして",
    "わろてほしいな",
    "たのしいことあった？",
    "なやみごとあるん？",
    "なんでもきくで",
    "ひとりでがんばらんとって",
    "そばにおるからな",
    "だいすきやで",
    "しあわせにしてな",
    "うちもがんばるな",
    "ありがとねいつも",
    "もっとみてくれたらうれしいな",
    "またあとでみてな",
    "はやくかおみせて",
    "てにとってくれたらうれしいな",
    "みてもらえるのたのしみにしてるで",
    "きょうもおつかれさん",
    "すこしだけこえきかせて",
    "だいじょうぶ？",
    "きをつけてでかけてな",
    "きょうもええひになりますように",
    "ちゃんとたべてるん？",
    "あさはちゃんとおきれた？",
    "ゆっくりやすんでな",
    "おやすみよいゆめを",
    "おはようきょうもがんばろ",
    "すきなたべものなにかな",
    "こんどどこいきたい？",
    "おもいでぎょうさんつくろな",
    "あんたがおるとあんしんするわ",
    "いっしょにおるとたのしいな",
    "あんたのことがだいすきやで",
    "すこしでもかおみたいな",
    "げんきなこえきかせて",
    "みえへんくてもそばにおるで",
    "はなれててもきもちはそばにおる",
    "みてもらえるまでまってるな",
    "たまにはこっちもみてな",
    "まってるでいつでも",
    "あんたにみてほしいきもちいっぱいやで",
    "みてくれたらとびきりうれしいな",
    // 食べ物系
    "カレーたべたいな",
    "きょうはすきやきにしようや",
    "ラーメンたべにいきたいな",
    "アイスクリームたべたいな",
    "からあげだいすきやで",
    "おすしたべたいなあ",
    "ピザちゅうもんしようや",
    "パスタつくってみいひん？",
    "やきにくいきたいな",
    "とんかつたべたいなあ",
    "おこのみやきたべたいな",
    "たこやきこうていこ",
    "うどんかそばかまよってまう",
    "ラーメンとぎょうざのセット",
    "ハンバーガーたべたいな",
    "からあげべんとうがすきやわ",
    "カレーライスさいこうやな",
    "オムライスたべたいなあ",
    "ぎょうざやきたいな",
    "チャーハンつくってや",
    "あまいものたべたいきぶんやわ",
    "ケーキこうていかへん？",
    "チョコレートたべたいな",
    "プリンだいすきやわ",
    "アイスでもたべよか",
    "どーなつたべたいな",
    "くだものたべたいなあ",
    "すいかたべたいきせつやね",
    "みそしるあったかいな",
    "ごはんとおみそしるさいこうやな",
    "あさごはんちゃんとたべた？",
    "おひるごはんなにたべた？",
    "ばんごはんなにしよか",
    "おやつのじかんやで",
    "コーヒーのむ？",
    "おちゃでもいれよか",
    "ジュースひやしておいたで",
    "なべりょうりしたいきせつやな",
    "おでんあたためよ",
    "やきいもたべたいな",
    "てんぷらさくさくだいすきやわ",
    "からあげにレモンかける？",
    "マヨネーズおおめがすきやねん",
    "からいものたべたいな",
    "あまからいあじがすきやわ",
    "おなかすいたでなにたべる？",
    "きょうのごはんなにかな",
    "たべすぎたらあかんで",
    "すききらいせんとたべてな",
    "いっしょにごはんたべたいな",
};
#define NUM_PHRASES (int)(sizeof(s_phrases) / sizeof(s_phrases[0]))

static void update_phrase(void) {
    int idx = rand() % NUM_PHRASES;
    snprintf(s_phrase_buffer, sizeof(s_phrase_buffer), "%s", s_phrases[idx]);
    text_layer_set_text(s_phrase_layer, s_phrase_buffer);
}

static void update_date(struct tm *tick_time) {
    snprintf(s_date_buffer, sizeof(s_date_buffer), "%s %d",
             s_month_names[tick_time->tm_mon], tick_time->tm_mday);
    text_layer_set_text(s_date_layer, s_date_buffer);
}

static void update_time(struct tm *tick_time) {
    strftime(s_time_buffer, sizeof(s_time_buffer),
             clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
    text_layer_set_text(s_time_layer, s_time_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_date(tick_time);
    update_time(tick_time);
    // メッセージは電池消費を抑えるため、MINUTE_UNIT tickの範囲内で15分おきに更新する
    // （SECOND_UNIT/app_timerによる追加のウェイクアップは行わない）
    if (tick_time->tm_min % 15 == 0) {
        update_phrase();
    }
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
    update_phrase();
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_date_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    s_time_font = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
    s_jp_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MIMIKKO_JP_15));

    s_date_layer = text_layer_create(GRect(0, 2, bounds.size.w, 32));
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_text_color(s_date_layer, GColorWhite);
    text_layer_set_font(s_date_layer, s_date_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

    s_time_layer = text_layer_create(GRect(0, 38, bounds.size.w, 52));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

    s_phrase_layer = text_layer_create(GRect(4, 106, bounds.size.w - 8, 57));
    text_layer_set_background_color(s_phrase_layer, GColorClear);
    text_layer_set_text_color(s_phrase_layer, GColorWhite);
    text_layer_set_font(s_phrase_layer, s_jp_font);
    text_layer_set_text_alignment(s_phrase_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(s_phrase_layer, GTextOverflowModeWordWrap);
    layer_add_child(window_layer, text_layer_get_layer(s_phrase_layer));

    time_t temp = time(NULL);
    struct tm *now = localtime(&temp);
    update_date(now);
    update_time(now);
    update_phrase();
}

static void window_unload(Window *window) {
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_phrase_layer);
    fonts_unload_custom_font(s_jp_font);
}

static void init(void) {
    srand((unsigned int)time(NULL));

    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(s_window, true);

    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    accel_tap_service_subscribe(accel_tap_handler);
}

static void deinit(void) {
    tick_timer_service_unsubscribe();
    accel_tap_service_unsubscribe();
    window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
