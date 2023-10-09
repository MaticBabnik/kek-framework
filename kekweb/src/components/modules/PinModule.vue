<script lang="ts" setup>
import { computed } from "@vue/reactivity";
import { useKekStore } from "../../stores/kekStore";
import { ref } from "vue";

import ToggleButton from "../toggle-button.vue";

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface PinData {
    state: boolean;
}

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === "string") return undefined;
    const pd = moduleData as PinData;

    if (pd?.state) {
        buttonState.value = true;
    }

    return pd;
});

const buttonState = ref(false);

async function toggle(val: boolean) {
    const state = val;
    buttonState.value = state;

    try {
        const resp = await kek.writeAndUpdate(props.modId, { state });
    } catch {
        buttonState.value = !state; //revert buttonState
    }
}
</script>

<template>
    <div class="mod mod-1x1 pin-module" :class="{ error: !data }">
        <h2>{{ props.modId }}</h2>
        <div v-if="data" class="data">
            <toggle-button :value="buttonState" @update:value="toggle" />
        </div>
        <h1 v-else>Error</h1>
    </div>
</template>

<style lang="less">
.pin-module {
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

    .data {
        flex: 1;
        display: grid;
        place-items: center;
    }
}
</style>
