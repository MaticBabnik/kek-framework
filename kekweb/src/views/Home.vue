<script lang="ts" setup >
import { useRouter } from 'vue-router';
const r = useRouter();

if (!localStorage.getItem('username') || !localStorage.getItem('password')) {
    r.replace('/login');
}


</script>

<template>
    <div>
        <h1>Home</h1>
        <p>Imagine a really epic UI that does stuff here</p>
    </div>
</template>

<style lang="less">

</style>