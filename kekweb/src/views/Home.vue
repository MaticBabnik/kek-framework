<script lang="ts" setup>
import { computed } from "@vue/reactivity";
import { useKekStore } from "../stores/kekStore";
import modRegistry from "../components/modules";
import { useIntervalFn } from "@vueuse/core";

const kek = useKekStore();

kek.init();

const modules = computed(() => {
    return Object.entries(kek.modules)
        .map(([id, type]) => ({
            id,
            component: `${type}Module`,
        }))
        .filter((m) => m.component in modRegistry);
});

useIntervalFn(() => {
    kek.readAll();
}, 10000);

//@ts-ignore
window.kek = kek;
</script>

<template>
    <div class="main">
        <div id="modules">
            <component
                v-for="mod in modules"
                :is="mod.component"
                :modId="mod.id"
            />
            <InfoModule />
        </div>
    </div>
</template>

<style lang="less">
#modules {
    display: grid;
    flex: 1;
    align-content: center;
    grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
    grid-gap: 0.5rem;
    grid-auto-rows: minmax(200px, auto);
    grid-auto-flow: dense;
    width: 80vw;
    padding: 2rem 0;
}

.main {
    display: flex;
    flex-direction: column;

    align-items: center;
}
</style>
