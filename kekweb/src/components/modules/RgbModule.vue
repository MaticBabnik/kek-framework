<script lang="ts" setup>
import { computed } from "@vue/reactivity";
import { useKekStore } from "../../stores/kekStore";
import ColorPicker from "../color-picker.vue";
import { ref, watch } from "vue";

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface RgbData {
    state: [number, number, number];
}

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === "string") return undefined;
    const pd = moduleData as RgbData;

    return pd;
});


const color = ref<[number, number, number]>(data.value?.state ?? [0, 0, 0]);

watch(data, (d) => {
    color.value = d?.state ?? [0, 0, 0];
});

async function set(nc: [number, number, number]) {
    try {
        const resp = await kek.writeAndUpdate(props.modId, { state: nc });
    } catch {
        color.value = data.value?.state ?? [0, 0, 0];
    }
}

</script>

<template>
    <div class="mod mod-2x2 rgb-module" :class="{ error: !data }">
        <h2>{{ props.modId }}</h2>
        <color-picker class="data" v-if="data" v-model:color="color" @update:color="set" />
        <h1 v-else>Error</h1>
    </div>
</template>

<style lang="less">
.rgb-module {
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
    }
}
</style>
