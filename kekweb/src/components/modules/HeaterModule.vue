<script lang="ts" setup>
import { computed } from "@vue/reactivity";
import { useKekStore } from "../../stores/kekStore";
import ToggleButton from "../toggle-button.vue";
import { ref, watch } from "vue";

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface HeaterData {
    enabled: boolean;
    temperature: {
        cur: number;
        set: number;
        on: boolean;
    };
    humidity: {
        cur: number;
        set: number;
        on: boolean;
    };
}

const ena = ref(false),
    st = ref(0),
    sh = ref(0);

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === "string") return undefined;

    return moduleData as HeaterData;
});

const dirty = computed(() => {
    if (!data.value) return false;
    return !(
        data.value.enabled == ena.value &&
        data.value.temperature.set == st.value &&
        data.value.humidity.set == sh.value
    );
});

watch(data, (val, ov) => {
    if (!val) return;

    if (
        val.enabled == ov?.enabled &&
        val.humidity.set == ov.humidity.set &&
        val.temperature.set == ov.temperature.set
    )
        return; // dont update if the set values didn't change
    //this mostly just prevents the read interval from interrupting your input

    ena.value = val.enabled;
    st.value = val.temperature.set;
    sh.value = val.humidity.set;
});

async function update() {
    console.log(
        await kek.writeAndUpdateSane(props.modId, {
            enabled: ena.value,
            setTemperature: st.value,
            setHumidity: sh.value,
        })
    );
}
</script>

<template>
    <div class="mod mod-2x2 dht-module" :class="{ error: !data }">
        <h2>{{ props.modId }}</h2>
        <div class="side-by-side" v-if="data">
            <div class="current">
                <span class="label">Temperature</span>

                <span class="temp-big" :class="{ glow: data.temperature.on }"
                    ><span>{{ data.temperature.cur.toFixed(1) }}</span>
                    &deg;C</span
                >

                <span class="label">Humidity</span>
                <span class="mid" :class="{ glow: data.humidity.on }"
                    ><span>{{ data.humidity.cur.toFixed(0) }}</span> %</span
                >
            </div>
            <div class="settings" :class="{ dirty }">
                <span class="label">Enabled:</span>
                <ToggleButton v-model:value="ena" />
                <span class="label">Target temperature:</span>
                <input
                    type="number"
                    v-model="st"
                    min="20"
                    max="35"
                    step="0.1"
                />
                <span class="label">Target humidity:</span>
                <input type="number" v-model="sh" min="20" max="80" />

                <button class="button" :disabled="!dirty" @click="update">
                    Apply
                </button>
            </div>
        </div>
        <h1 v-else>Error</h1>
    </div>
</template>

<style lang="less">
.dht-module {
    &.error {
        background-color: tomato;
        h1 {
            text-align: center;
            margin-top: 3rem;
        }
    }

    h2 {
        margin: 0;
        text-align: center;
    }

    display: flex;
    flex-direction: column;

    .side-by-side {
        display: flex;
        flex: 1;
        flex-direction: row;
    }

    .label {
        font-weight: 500;
        font-size: 1.1rem;
        &:not(:first-of-type) {
            margin-top: 0.5rem;
        }
    }

    .settings {
        flex: 1;
        display: flex;
        flex-direction: column;

        justify-content: center;
        align-items: center;

        .toggle {
            margin: 0.5rem;
        }

        input[type="number"] {
            font-size: 16px;
            padding: 0.5rem;
            width: 4rem;
            border-radius: 5px;
            margin: 0.5rem;
        }

        &.dirty > .button {
            opacity: 1;
            transition: 0.1s ease;
        }

        .button {
            opacity: 0;
            transition: 0.1s ease;
        }
    }

    .current {
        flex: 1;
        display: flex;
        flex-direction: column;

        justify-content: center;
        align-items: center;

        .temp-big {
            font-size: 2rem;
            line-height: 4rem;

            span {
                font-size: 4rem;
                font-weight: bold;
            }
        }

        .mid {
            font-size: 1.5rem;
            line-height: 3rem;

            span {
                font-size: 3rem;
                font-weight: bold;
            }
        }

        .glow {
            text-shadow: red 0 0 5px;
        }
    }
}
</style>
