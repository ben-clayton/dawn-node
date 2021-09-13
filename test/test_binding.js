const DawnNode = require("../build/dawnnode");

const assert = require("assert");

async function testGPUArrayCopy() {
  const gpu = navigator.gpu;
  const adapter_promise = gpu.requestAdapter();
  const adapter = await adapter_promise;
  if (!adapter) {
    console.log("No gpu adapter returned!");
  }
  const device = await adapter.requestDevice();

  const gpuWriteBuffer = device.createBuffer({
    mappedAtCreation: true,
    size: 4,
    usage: GPUBufferUsage.COPY_SRC | GPUBufferUsage.MAP_WRITE,
  });
  const writeBuffer = gpuWriteBuffer.getMappedRange();

  new Uint8Array(writeBuffer).set([0, 1, 2, 3]);

  gpuWriteBuffer.unmap();

  const gpuReadBuffer = device.createBuffer({
    size: 4,
    usage: GPUBufferUsage.COPY_DST | GPUBufferUsage.MAP_READ,
  });

  const copyEncoder = device.createCommandEncoder();
  copyEncoder.copyBufferToBuffer(
    gpuWriteBuffer /* source buffer */,
    0 /* source offset */,
    gpuReadBuffer /* destination buffer */,
    0 /* destination offset */,
    4 /* size */
  );

  const copyCommands = copyEncoder.finish();
  device.queue.submit([copyCommands]);

  await gpuReadBuffer.mapAsync(GPUMapMode.READ);
  const copyArrayBuffer = gpuReadBuffer.getMappedRange();
  assert.deepEqual(Array.from(new Uint8Array(copyArrayBuffer)), [0, 1, 2, 3]);
}

assert.doesNotThrow(
  testGPUArrayCopy,
  undefined,
  "testBasic threw an exception"
);
