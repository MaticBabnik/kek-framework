<script setup lang="ts">
import { onMounted, onUnmounted, ref } from "vue";

const props = defineProps<{
    color: [number, number, number];
}>();

const emit = defineEmits<{
    (e: "update:color", color: [number, number, number]): void;
}>();

const wheel = ref<HTMLDivElement>();
const picker = ref<HTMLDivElement>();

const l = ref(0.5);
const t = ref(0.5);

function vdiv(v1: number[], v2: number[]): number[] {
    return v1.map((x, i) => x / v2[i]);
}

onMounted(() => {
    wheel.value?.addEventListener("mousedown", (e) => {
        //@ts-ignore
        let pos = [e.layerX, e.layerY];
        let size = [
            wheel.value?.clientWidth,
            wheel.value?.clientHeight,
        ] as number[];
        let rp = vdiv(pos, size);

        console.log(rp);
        [l.value, t.value] = rp;
    });
});
onUnmounted(() => {});
</script>

<template>
    <div class="color-picker">
        <div ref="wheel" class="colorwheel">
            <div
                ref="picker"
                class="picker"
                :style="{ left: `${l * 100}%`, top: `${t * 100}%` }"
            ></div>
        </div>
        <input type="range" />
    </div>
</template>

<style lang="less">
.color-picker {
    .colorwheel {
        height: 80%;
        aspect-ratio: 1;
        background-color: #000;
        border-radius: 50%;

        position: relative;

        color: hsl(0, 100%, 50%);
        background: radial-gradient(circle, #fff, #0000 75%),
            conic-gradient(
                hsl(0, 100%, 50%) 0deg,
                hsl(30, 100%, 50%) 30deg,
                hsl(60, 100%, 50%) 60deg,
                hsl(90, 100%, 50%) 90deg,
                hsl(120, 100%, 50%) 120deg,
                hsl(150, 100%, 50%) 150deg,
                hsl(180, 100%, 50%) 180deg,
                hsl(210, 100%, 50%) 210deg,
                hsl(240, 100%, 50%) 240deg,
                hsl(270, 100%, 50%) 270deg,
                hsl(300, 100%, 50%) 300deg,
                hsl(330, 100%, 50%) 330deg,
                hsl(360, 100%, 50%) 360deg
            );

            cursor: crosshair;
        .picker {
            position: absolute;
            top: 25%;
            left: 25%;
            transform: translate(-50%, -50%);
            width: 0.5rem;
            height: 0.5rem;
            border-radius: 50%;
            border: 1px solid #fff;
            outline: 1px solid #000;
            pointer-events: none;
        }
    }
}
</style>
