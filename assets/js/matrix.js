/** Browser implementation of the console's row-equilibrated square-matrix methods. */
const EPS = 1e-9;

function parseMatrix(text) {
  const rows = text.trim().split(/[;\n]+/).filter(line => line.trim());
  if (!rows.length || rows.length > 10) throw new Error("Enter a square matrix with 1 to 10 rows.");
  const matrix = rows.map(line => {
    const tokens = line.includes(",") ? line.split(",") : line.trim().split(/\s+/);
    return tokens.map(token => {
      const value = Number(token.trim());
      if (!token.trim() || !Number.isFinite(value)) throw new Error("Every entry must be a finite number.");
      return value;
    });
  });
  if (matrix.some(row => row.length !== rows.length)) throw new Error("The number of columns must match the number of rows.");
  return matrix;
}

function transpose(matrix) {
  return matrix[0].map((_, column) => matrix.map(row => row[column]));
}
function equilibrate(matrix) {
  const scales = matrix.map(row => Math.max(...row.map(Math.abs)));
  return {scales, rows: matrix.map((row, i) => row.map(value => scales[i] ? value / scales[i] : value))};
}
function swapPivot(rows, column, target) {
  let pivot = target;
  for (let row = target + 1; row < rows.length; row++) {
    if (Math.abs(rows[row][column]) > Math.abs(rows[pivot][column])) pivot = row;
  }
  if (Math.abs(rows[pivot][column]) < EPS) return null;
  [rows[target], rows[pivot]] = [rows[pivot], rows[target]];
  return pivot !== target;
}
function multiplyScaled(product, factor) {
  const exponent = Math.max(-1022, Math.min(1023, Math.floor(Math.log2(Math.abs(factor))) + 1));
  product.fraction *= factor / Math.pow(2, exponent);
  const normalization = Math.floor(Math.log2(Math.abs(product.fraction))) + 1;
  product.fraction /= Math.pow(2, normalization);
  product.exponent += exponent + normalization;
}
function scaledValue(product) {
  if (product.exponent > 1023) return (product.fraction * Math.pow(2, product.exponent - 1023)) * Math.pow(2, 1023);
  if (product.exponent < -1022) return (product.fraction * Math.pow(2, product.exponent + 1022)) * Math.pow(2, -1022);
  return product.fraction * Math.pow(2, product.exponent);
}
function determinant(matrix) {
  const {rows, scales} = equilibrate(matrix);
  if (scales.some(scale => scale === 0)) return 0;
  const product = {fraction: 1, exponent: 0};
  scales.forEach(scale => multiplyScaled(product, scale));
  for (let column = 0; column < rows.length; column++) {
    const swapped = swapPivot(rows, column, column);
    if (swapped === null) return 0;
    if (swapped) product.fraction = -product.fraction;
    multiplyScaled(product, rows[column][column]);
    for (let row = column + 1; row < rows.length; row++) {
      const factor = rows[row][column] / rows[column][column];
      for (let c = column; c < rows.length; c++) rows[row][c] -= factor * rows[column][c];
    }
  }
  return scaledValue(product);
}
function reducedAugmented(matrix, right) {
  const {rows, scales} = equilibrate(matrix);
  if (scales.some(scale => scale === 0)) throw new Error("The matrix has no unique inverse or solution.");
  const augmented = rows.map((row, i) => [...row, ...right[i].map(value => value / scales[i])]);
  for (let column = 0; column < matrix.length; column++) {
    if (swapPivot(augmented, column, column) === null) throw new Error("The matrix has no unique inverse or solution.");
    const divisor = augmented[column][column];
    augmented[column] = augmented[column].map(value => value / divisor);
    for (let row = 0; row < matrix.length; row++) {
      if (row === column) continue;
      const factor = augmented[row][column];
      augmented[row] = augmented[row].map((value, c) => value - factor * augmented[column][c]);
    }
  }
  return augmented.map(row => row.slice(matrix.length));
}
function inverse(matrix) {
  const identity = matrix.map((row, i) => row.map((_, j) => i === j ? 1 : 0));
  return reducedAugmented(matrix, identity);
}
function rank(matrix) {
  const {rows} = equilibrate(matrix);
  let target = 0;
  for (let column = 0; column < matrix.length && target < matrix.length; column++) {
    if (swapPivot(rows, column, target) === null) continue;
    const divisor = rows[target][column];
    rows[target] = rows[target].map(value => value / divisor);
    for (let row = 0; row < rows.length; row++) {
      if (row === target) continue;
      const factor = rows[row][column];
      rows[row] = rows[row].map((value, c) => value - factor * rows[target][c]);
    }
    target++;
  }
  return target;
}
function adjugate(matrix) {
  if (matrix.length === 1) return [[1]];
  return matrix.map((row, i) => row.map((_, j) => {
    const minor = matrix.filter((_, r) => r !== j).map(values => values.filter((_, c) => c !== i));
    return ((i + j) % 2 ? -1 : 1) * determinant(minor);
  }));
}
function calculate(operation, matrix, second, scalar = 1) {
  const n = matrix.length;
  const unary = {
    display: () => matrix.map(row => [...row]),
    transpose: () => transpose(matrix),
    determinant: () => determinant(matrix),
    inverse: () => inverse(matrix),
    adjugate: () => adjugate(matrix),
    trace: () => matrix.reduce((sum, row, i) => sum + row[i], 0),
    rank: () => rank(matrix),
    properties: () => ({
      symmetric: matrix.every((row, i) => row.every((value, j) => Math.abs(value - matrix[j][i]) <= EPS)),
      identity: matrix.every((row, i) => row.every((value, j) => Math.abs(value - (i === j ? 1 : 0)) <= EPS))
    }),
    scale: () => {
      if (!Number.isFinite(scalar)) throw new Error("The scalar must be finite.");
      return matrix.map(row => row.map(value => value * scalar));
    }
  };
  if (unary[operation]) return ensureFinite(unary[operation]());
  if (operation === "solve") {
    if (!second || second.length !== n || second.some(row => row.length !== 1)) throw new Error("Enter one value per row for vector b.");
    return ensureFinite(reducedAugmented(matrix, second));
  }
  if (!["add", "subtract", "multiply"].includes(operation)) throw new Error("Choose a supported operation.");
  if (!second || second.length !== n || second.some(row => row.length !== n)) throw new Error("Matrix B must have the same square dimensions as A.");
  const result = matrix.map((row, i) => row.map((value, j) => {
    if (operation === "add") return value + second[i][j];
    if (operation === "subtract") return value - second[i][j];
    return row.reduce((sum, entry, k) => sum + entry * second[k][j], 0);
  }));
  return ensureFinite(result);
}
function ensureFinite(value) {
  if (Array.isArray(value)) return value.map(ensureFinite);
  if (typeof value === "number" && !Number.isFinite(value)) throw new Error("This result exceeds floating-point range. Rescale the input.");
  return value;
}

globalThis.MatrixCalculator = {EPS, parseMatrix, transpose, determinant, inverse, rank, adjugate, calculate};
