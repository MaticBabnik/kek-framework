<script lang="ts" setup>
import { computed } from '@vue/reactivity';
import { useKekStore } from '../../stores/kekStore';

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface DhtData {
    temperature: number,
    humidity: number,
    heat_index: number
}

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === 'string') return undefined;

    return moduleData as DhtData;
})
</script>
    
<template>
    <div class="mod mod-2x1 dht-module" :class="{error:!data}">
        <h2>{{props.modId}}</h2>

        <div v-if="data" class="data">
            <span class="label">Temperature</span>

            <span class="temp-big"><span>{{data.temperature.toFixed(1)}}</span> &deg;C</span>

            <span class="label">Humidity</span>
            <span class="mid"><span>{{data.humidity.toFixed(0)}}</span> %</span>

            <span class="label">Feels like</span>
            <span class="mid"><span>{{data.heat_index.toFixed(1)}}</span> &deg;C</span>

        </div>
        <h1 v-else>
            Error
        </h1>
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

    .data {
        flex: 1;
        display: flex;
        flex-direction: column;

        justify-content: center;
        align-items: center;

        .label {
            font-weight: 500;
            font-size: 1.1rem;
            &:not(:first-of-type) {
                margin-top: .5rem;
            }
        }

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
    }
}
</style>