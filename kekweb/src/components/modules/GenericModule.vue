<script lang="ts" setup>
import { computed } from '@vue/reactivity';
import { useKekStore } from '../../stores/kekStore';

const props = defineProps<{ modId: string }>();
const kek = useKekStore();

const data = computed(() => {
    return kek.data[props.modId];
})
</script>

<template>
    <div class="mod mod-2x2 generic-module">
        <h2>{{props.modId}}</h2>
        <pre>{{data}}</pre>
    </div>
</template>

<style lang="less">
.info-module {
    text-align: center;
    display: grid;
    place-items: center;
}
</style>