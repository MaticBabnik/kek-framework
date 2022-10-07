<script lang="ts" setup>
import { computed } from '@vue/reactivity';
import { useKekStore } from '../../stores/kekStore';
import { ref } from 'vue';

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface PinData {
    state: boolean
}

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === 'string') return undefined;
    const pd = moduleData as PinData;

    if (pd?.state) {
        buttonState.value = true;
    }

    return pd;
})

const buttonState = ref(false);

async function toggle() {
    const state = !buttonState.value;
    buttonState.value = state;

    try {
        const resp = await kek.writeAndUpdate(props.modId, { state });
    } catch {
        buttonState.value = !state; //revert buttonState
    }
}
</script>
    
<template>
    <div class="mod mod-1x1 pin-module" :class="{error:!data}">
        <h2>{{props.modId}}</h2>
        <div v-if="data" class="data">
            <button class="toggle" :class="{active:buttonState}" @click="toggle">
                <div class="circle"></div>
            </button>
        </div>
        <h1 v-else>
            Error
        </h1>
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

        .toggle {
            width: 4rem;
            height: 2rem;
            border-radius: 1rem;

            border: none;
            outline: none;
            position: relative;

            background-color: #242424;

            .circle {
                position: absolute;

                height: 1.5rem;
                width: 1.5rem;
                border-radius: .75rem;

                top: 0.25rem;
                left: 0.25rem;

                background-color: white;

                transition: 0.2s cubic-bezier(0.5, 0, 0.5, 1);
            }

            &.active .circle {
                left: 2.25rem;
                background-color: coral;
            }
        }
    }
}
</style>