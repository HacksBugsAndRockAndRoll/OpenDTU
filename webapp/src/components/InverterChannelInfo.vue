<template>
    <div class="card" :class="{ 'border-info': channelNumber == 0 }">
        <div v-if="channelNumber >= 1" class="card-header">String {{ channelNumber }}</div>
        <div v-if="channelNumber == 0" class="card-header bg-info">Phase {{ channelNumber + 1 }}</div>
        <div class="card-body">
            <table class="table table-striped table-hover">
                <thead>
                    <tr>
                        <th scope="col">Property</th>
                        <th style="text-align: right" scope="col">Value</th>
                        <th scope="col">Unit</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="(property, key) in channelData" :key="`prop-${key}`">
                        <template v-if="property">
                            <th scope="row">{{ key }}</th>
                            <td style="text-align: right">{{ formatNumber(property.v) }}</td>
                            <td>{{ property.u }}</td>
                        </template>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { InverterStatistics } from '@/types/LiveDataStatus';

export default defineComponent({
    props: {
        channelData: { type: Object as PropType<InverterStatistics>, required: true },
        channelNumber: { type: Number, required: true },
    },
    methods: {
        formatNumber(num: number) {
            return new Intl.NumberFormat(
                undefined, { minimumFractionDigits: 2, maximumFractionDigits: 2 }
            ).format(num);
        },
    },
});
</script>
