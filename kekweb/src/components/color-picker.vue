<script setup lang="ts">
import { onMounted, onUnmounted, ref, watch } from "vue";
import { HSVtoRGB, RGBtoHSV, toCssHSV, toCssRGB } from "../util/color";




const props = defineProps<{
    color: [number, number, number];
}>();

const emit = defineEmits<{
    (e: "update:color", color: [number, number, number]): void;
}>();

const wheel = ref<HTMLDivElement>();
const picker = ref<HTMLDivElement>();
const nomouse = ref(false);

const circleX = ref(0.5);
const circleY = ref(0);
const value = ref(1);

//helper functions -----------
function vdiv(v1: number[], v2: number[]): number[] {
    return v1.map((x, i) => x / v2[i]);
}

function clamp(min: number, val: number, max: number) {
    return Math.max(min, Math.min(val, max));
}

// ------

watch(props.color, (v) => {
    console.log("new");
    setupFromRgb(v);
})

function setupFromRgb(color: [number, number, number]) {
    const [h, s, v] = RGBtoHSV(color);

    const hRad = h * 2 * Math.PI; //convert hue to radians
    const x = (Math.cos(hRad) * s) / 2 + 0.5,
        y = (Math.sin(hRad) * s) / 2 + 0.5;

    circleX.value = clamp(0, x, 1);
    circleY.value = clamp(0, y, 1);

    value.value = v;
}

function syncToRgb(ix: number, iy: number, v: number) {
    console.log({ ix, iy, v })

    const [x, y] = [(ix - .5) * 2, (iy - 0.5) * 2]; //remap [0,1] to [-1,1]

    // get the hue angle
    let h = Math.atan2(y, x) / Math.PI / 2;
    if (h < 0) h++;

    //get the saturation
    let s = Math.sqrt(x * x + y * y);

    const rgb = HSVtoRGB(h, s, v);
    emit("update:color", rgb);
}

setupFromRgb(props.color);

function onDown(e: MouseEvent) {
    wheel.value?.addEventListener("mousemove", update);
    window.addEventListener("mouseup", onUp);
    update(e);
    nomouse.value = true;
}

function update(e: MouseEvent) {
    //@ts-ignore
    let pos = [e.layerX, e.layerY];
    let size = [
        wheel.value?.clientWidth,
        wheel.value?.clientHeight,
    ] as number[];

    let rp = vdiv(pos, size);

    [circleX.value, circleY.value] = rp;
}

function onUp() {
    wheel.value?.removeEventListener("mousemove", update);
    window.removeEventListener("mouseup", onUp);
    nomouse.value = false;

    syncToRgb(circleX.value, circleY.value, value.value);
}

onMounted(() => {
    wheel.value?.addEventListener("mousedown", onDown);
    // wheel.value?.addEventListener("touchstart",onDown);
});
onUnmounted(() => {
    window.removeEventListener("mouseup", onUp);
    wheel.value?.removeEventListener("mousedown", onDown);
    wheel.value?.removeEventListener("mousemove", update);
    nomouse.value = false;
});
</script>

<template>
    <div class="color-picker">
        <div ref="wheel" class="colorwheel" :class="{nomouse}">
            <div ref="picker" class="picker" :style="{ left: `${circleX * 100}%`, top: `${circleY * 100}%` }"></div>
        </div>
        <input type="range" min="0" step="0.01" max="1" v-model="value" @change="syncToRgb(circleX, circleY, value);" />
        <div class="preview" :style="{backgroundColor:toCssRGB(props.color)}">

        </div>
    </div>
</template>

<style lang="less">
.color-picker {
    display: grid;

    grid-template-columns: auto 2.5rem;
    grid-template-rows: auto auto;
    grid-template-areas: "picker picker"
        "value preview";

    grid-gap: .5rem;

    align-content: center;
    justify-content: center;

    .colorwheel {
        grid-area: picker;

        &.nomouse {
            cursor: none;
        }

        touch-action: none;

        height: 15rem;
        aspect-ratio: 1;
        background-color: #000;
        border-radius: 50%;

        position: relative;

        color: hsl(0, 100%, 50%);
        background: radial-gradient(circle, #fff, #fff0 75%),
        conic-gradient(hsl(270, 100%, 50%) 0deg,
            hsl(300, 100%, 50%) 30deg,
            hsl(330, 100%, 50%) 60deg,
            hsl(0, 100%, 50%) 90deg,
            hsl(30, 100%, 50%) 120deg,
            hsl(60, 100%, 50%) 150deg,
            hsl(90, 100%, 50%) 180deg,
            hsl(120, 100%, 50%) 210deg,
            hsl(150, 100%, 50%) 240deg,
            hsl(180, 100%, 50%) 270deg,
            hsl(210, 100%, 50%) 300deg,
            hsl(240, 100%, 50%) 330deg,
            hsl(270, 100%, 50%) 360deg);

        .picker {
            position: absolute;
            top: 25%;
            left: 25%;
            transform: translate(-50%, -50%);
            width: 0.5rem;
            height: 0.5rem;
            border-radius: 50%;
            border: 2px solid #fff;
            outline: 2px solid #000;
            pointer-events: none;
        }
    }

    input {
        color: transparent;
        background-color: transparent;
        appearance: none;
        margin: 0;
        &::-webkit-slider-runnable-track {
            width: 100%;
            height: 2rem;
            background: linear-gradient(to right, #000, #fff);
            border-radius: .5rem;
        }

        &::-webkit-slider-thumb {
            border-left: 2px solid #000;
            height: 100%;
            width: 4px;
            background: #fff;
            cursor: pointer;
            appearance: none;
        }

        grid-area: value;
    }

    .preview {
        grid-area: preview;
        width: 2rem;
        height: 2rem;

        border-radius: 50%;
        outline: 2px solid white;
    }
}
</style>
