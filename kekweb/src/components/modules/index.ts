import InfoModule from "./InfoModule.vue";
import GenericModule from "./GenericModule.vue";
import DhtModule from "./DhtModule.vue";
import PinModule from "./PinModule.vue";
import ESP32Module from "./ESP32Module.vue"


export default {
    InfoModule,
    DhtModule,
    PinModule,
    RgbLedModule: GenericModule,
    ESP32Module,
}