<script lang="ts" setup>
import { computed } from '@vue/reactivity';
import { useKekStore } from '../../stores/kekStore';

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

interface SystemData {
    heap: {
        free: number,
        total: number
    },
    chip: {
        cores: number,
        model: string,
        rev: number,
        freq: number,
    },
    uptime: number
}

const data = computed(() => {
    const moduleData = kek.data[props.modId];
    if (typeof moduleData === 'string') return undefined;

    return moduleData as SystemData;
})

const MINUTE = 60, HOUR = 3600, DAY = HOUR * 24;
function fuzzyUptime(s: number) {
    const mf = Math.floor;

    const days = mf(s / DAY);
    s = s % DAY;
    const hours = mf(s / HOUR);
    s = s % HOUR;
    const minutes = mf(s / MINUTE);
    const seconds = s % MINUTE;

    let ret = "";
    if (days > 0) ret += `${days}d`
    if (hours > 0) ret += ` ${hours}h`
    if (days > 0) return ret;
    if (minutes > 0) ret += ` ${minutes}m`
    if (hours != 0) return ret
    else return `${ret} ${seconds}s`;
}

function heapFormat(mem: number) {
    if (mem > 1024) return `${Math.floor(mem / 1024)}k`
    else return `${mem} !`;
}

</script>
    
<template>
    <div class="mod mod-2x2 esp32-module" :class="{error:!data}">
        <h2>{{props.modId}}</h2>

        <div v-if="data" class="data">
            <p class="esp">
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 119 169">
                    <path fill="#000" d="M0 0h119v169H0z" />
                    <path fill="#B6B6B6" fill-rule="evenodd"
                        d="M7 46a1 1 0 0 1 1-1h103a1 1 0 0 1 1 1v114a1 1 0 0 1-1 1H8a1 1 0 0 1-1-1V46Zm9 110a5 5 0 1 0 0-10 5 5 0 0 0 0 10Z"
                        clip-rule="evenodd" />
                    <path stroke="#DECD87" stroke-linecap="round" stroke-linejoin="round" stroke-width="5"
                        d="M103 38V7H86.5M11 38V7h13.5v16h16V7h16v16H73V7h13.5m0 0v31" />
                    <path fill="#DECD87"
                        d="M0 46h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Z" />
                    <path fill="#DECD87"
                        d="M0 82h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm0 9h5v6H0v-6Zm103 10v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm-9 0v5h-6v-5h6Zm92-118h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Zm0 9h5v6h-5v-6Z" />
                </svg>
            <h2>{{data.chip.model}}</h2>
            <ul>
                <li>Cores: <span class="spec">{{data.chip.cores}}</span></li>
                <li>Clock: <span class="spec">{{data.chip.freq}}MHz</span></li>
                <li>Revision: <span class="spec">{{data.chip.rev}}MHz</span></li>
            </ul>

            </p>
            <div class="heap">
                <span class="static">Free heap:</span>
                <span class="size">{{heapFormat(data.heap.free)}} </span>
                <span class="static">Total:</span>
                <span class="size">{{heapFormat(data.heap.total)}} </span>
            </div>
            <div class="uptime">
                <span class="static">Uptime:</span>
                <span class="time">{{fuzzyUptime(data.uptime)}}</span>
            </div>
        </div>
        <h1 v-else>
            Error
        </h1>
    </div>
</template>
    
<style lang="less">
.esp32-module {
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
        display: grid;
        flex: 1;
        margin-top: .5rem;
        grid-template-columns: repeat(2, 1fr);
        grid-template-rows: repeat(2, 1fr);
        grid-gap: .5rem;
        grid-template-areas: "esp heap"
            "esp uptime";

        &>* {
            background-color: #242424;
            margin: 0;
            border-radius: .5rem;
        }

        .esp {
            grid-area: esp;
            padding-top: .5rem;

            ul {
                list-style: none;
                padding: 0 .5rem;
            }

            li {
                font-weight: bold;

                .spec {
                    font-weight: normal;
                    font-family: monospace;
                    float: right;
                }
            }

            svg {
                width: 100%;
                height: 150px;
                margin-bottom: .5rem;
            }

        }

        .heap {
            grid-area: heap;

        }

        .static {
            font-size: 1.2rem;
            margin: .5rem 0;
        }

        .uptime {
            grid-area: uptime;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;

            .time {
                font-size: 2rem;
            }
        }

        .heap {
            grid-area: heap;

            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;

            .size {
                font-family: monospace;
                font-size: 2rem;
            }
        }
    }
}
</style>