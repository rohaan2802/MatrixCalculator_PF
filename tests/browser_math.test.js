import test from "node:test";
import assert from "node:assert/strict";
import "../assets/js/matrix.js";
const {parseMatrix, determinant, inverse, adjugate, calculate, rank} = globalThis.MatrixCalculator;
const A = [[4,7],[2,6]];
test("known arithmetic and pivot sign", () => {
  assert.ok(Math.abs(determinant(A) - 10) < 1e-12);
  assert.equal(determinant([[0,2],[3,4]]), -6);
  assert.deepEqual(calculate("multiply", A, [[1,2],[3,4]]), [[25,36],[20,28]]);
  assert.deepEqual(calculate("solve", A, [[18],[14]]).map(row => Math.round(row[0])), [1,2]);
});
test("inverse residual and source preservation", () => {
  const copy = structuredClone(A);
  const product = calculate("multiply", A, inverse(A));
  product.forEach((row,i) => row.forEach((value,j) => assert.ok(Math.abs(value-(i===j?1:0)) < 1e-12)));
  assert.deepEqual(A, copy);
});
test("small scales and singular adjugate", () => {
  assert.equal(determinant([[Number.MIN_VALUE]]), Number.MIN_VALUE);
  assert.equal(determinant([[Number.MAX_VALUE]]), Number.MAX_VALUE);
  assert.ok(Math.abs(determinant([[1e-200,0,0,0],[0,1e-200,0,0],[0,0,1e200,0],[0,0,0,1e200]]) - 1) < 1e-12);
  assert.equal(rank([[1e-12,0],[0,2e-12]]), 2);
  assert.ok(Math.abs(inverse([[1e-12,0],[0,2e-12]])[0][0] / 1e12 - 1) < 1e-12);
  const singular4 = [[1,0,0,0],[0,2,0,0],[0,0,3,0],[0,0,0,0]];
  assert.equal(adjugate(singular4)[3][3], 6);
  assert.throws(() => inverse([[1,2],[2,4]]), /unique/);
});
test("parser rejects malformed and nonfinite inputs", () => {
  assert.deepEqual(parseMatrix("4,7\n2,6"), A);
  assert.deepEqual(parseMatrix("4 7;2 6"), A);
  for (const text of ["", "1,2\n3", "1,,2", "NaN", "Infinity", "1e999"]) assert.throws(() => parseMatrix(text));
  assert.throws(() => parseMatrix(Array(11).fill(Array(11).fill(1).join(" ")).join("\n")));
});
test("dimension errors and arithmetic overflow are visible", () => {
  assert.throws(() => calculate("add",A,[[1]]), /dimensions/);
  assert.throws(() => calculate("solve",A,[[1,2]]), /one value/);
  assert.throws(() => calculate("scale",[[1e308]],null,10), /range/);
});
