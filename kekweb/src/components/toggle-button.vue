<script setup lang="ts">
const props = defineProps<{
    value: boolean;
}>();

defineEmits<{
    (e: "update:value", value: boolean): void;
}>();
</script>

<template>
    <button
        class="toggle"
        :class="{ active: props.value }"
        @click="$emit('update:value', !props.value)"
    >
        <div class="circle"></div>
    </button>
</template>

<style lang="less">
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
        border-radius: 0.75rem;

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
</style>
