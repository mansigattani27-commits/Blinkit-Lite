# Blinkit-Lite
<br>


<h2>Blinkit-Lite Quick-Commerce</h2>
<p>
        Blinkit is a quick-commerce platform that delivers groceries and daily essentials in minutes by operating 
        a network of neighborhood dark stores (micro-fulfillment centers). It maintains a catalog of SKUs 
        (Stock Keeping Units) and manages customer orders within a promised ETA window. All data must be stored 
        in fixed-size arrays of structures.
</p>

<h3>Structures (use only these two)</h3>

<h4>1) SKU (master catalog; single dark store)</h4>
<ul>
        <li>skuId (<code>int</code>, unique)</li>
        <li>name (<code>string</code>)</li>
        <li>category (<code>string</code>)</li>
        <li>price (<code>float</code>)</li>
        <li>stock (<code>int</code>, current on-hand)</li>
        <li>soldCount (<code>int</code>; increase only on delivery)</li>
</ul>
 
<h4>2) Order</h4>
<ul>
    <li>orderId (<code>int</code>, unique)</li>
    <li>orderTime (<code>int</code> YYYYMMDDHHMM)</li>
    <li>status (<code>int</code>: 0 = Placed, 1 = Delivered, 2 = Cancelled)</li>
    <li>deliveryTime (<code>int</code>; 0 if N/A)</li>
    <li>itemIds [MAX_ITEMS] (<code>int</code>)</li>
    <li>itemQty [MAX_ITEMS] (<code>int</code>)</li>
    <li>itemCount (<code>int</code>)</li>
    <li>subtotal (<code>float</code>; sum of price × qty at order time)</li>
</ul>

<h3>Functions to Implement</h3>

<h4>1) Add_SKU()</h4>
<p>
    Add a new SKU to the <code>SKU[]</code> array ensuring:
</p>
<ul>
    <li><strong>skuId</strong> is unique</li>
    <li><strong>price</strong> is non-negative</li>
    <li><strong>stock</strong> is non-negative</li>
    <li>Maintain array integrity by using <strong>append or insert</strong> operation</li>
</ul>

<h4>2) Update_or_Delete_SKU(skuId)</h4>
<p>
    Update or delete a SKU identified by <code>skuId</code>:
</p>
<ul>
    <li>Allow updating of: <strong>name</strong>, <strong>category</strong>, <strong>price</strong>, <strong>stock</strong>.</li>
    <li>Allow delete only if no <strong>Delivered</strong> order references the SKU.</li>
    <li>On delete, shift the array left to keep continuity (no gaps in <code>SKUList</code>).</li>
</ul>

<h4>3) Place_Order(itemIds, itemQty, itemCount, orderTime)</h4>
<p>
    Create a new order with given items and time:
</p>
<ul>
    <li>Validate that all <strong>itemIds</strong> exist in <code>SKUList</code>.</li>
    <li>Validate that each <strong>qty &gt; 0</strong>.</li>
    <li>Compute <strong>subtotal</strong> from current SKU price × qty.</li>
    <li>Create an <code>Order</code> with <strong>status = Placed</strong> and store line items.</li>
    <li><strong>No stock change</strong> at this stage (we don’t reserve stock yet).</li>
</ul>

<h4>4) Deliver_Order(orderId, deliveryTime)</h4>
<p>
    Deliver an order if stock is sufficient at delivery time:
</p>
<ul>
    <li>Re-validate stock for each line item when delivering.</li>
    <li>If stock is sufficient for all items:
        <ul>
            <li>Decrement <code>stock</code> in the corresponding SKUs.</li>
            <li>Increment <code>soldCount</code> by the delivered quantity.</li>
            <li>Set <code>deliveryTime</code>.</li>
            <li>Set <code>status = Delivered</code>.</li>
        </ul>
    </li>
    <li>If stock is insufficient for any line, fail with a clear error/return code (no partial delivery).</li>
</ul>

<h4>5) Cancel_Order(orderId)</h4>
<p>
    Cancel a pending order:
</p>
<ul>
    <li>Allowed only if order is in <strong>Placed</strong> state.</li>
    <li>No stock change on cancel since stock was never reserved.</li>
    <li>Reject cancel request if order is already <strong>Delivered</strong>.</li>
</ul>

<h4>6) Search_SKU_ByName_Substring(query)</h4>
<p>
    Search SKUs by a substring of their name:
</p>
<ul>
    <li>Perform a <strong>linear</strong>, <strong>case-insensitive</strong> substring search over <code>SKU.name</code>.</li>
    <li>Return (or print) the indices of all matching SKUs.</li>
</ul>

<h4>7) Sort_Orders_ByTime()</h4>
<p>
    Sort orders chronologically:
</p>
<ul>
    <li>Sort the <code>OrderList</code> array by <code>orderTime</code> (earliest → latest).</li>
    <li>If two orders have the same <code>orderTime</code>, break ties by <code>orderId</code>.</li>
</ul>

<h4>8) TopK_Bestsellers(K)</h4>
<p>
    Find the top-K bestselling SKUs:
</p>
<ul>
    <li>Use <code>soldCount</code> to rank SKUs in <strong>descending</strong> order.</li>
    <li>Tie-break by <strong>name</strong> (lexicographically).</li>
    <li>Implement without modifying the original <code>SKUList</code> (use an index array / view).</li>
    <li>Print the top-K SKUs with relevant details (e.g., id, name, soldCount).</li>
</ul>

<h4>9) ABC_Analysis_SoldCount()</h4>
<p>
    Perform ABC analysis based on <code>soldCount</code>:
</p>
<ul>
    <li>Sort SKUs by <code>soldCount</code> in <strong>descending</strong> order.</li>
    <li>Compute cumulative percentage of total units sold.</li>
    <li>Label SKUs into categories:
        <ul>
            <li><strong>A</strong>: ≈ 80% of total units</li>
            <li><strong>B</strong>: ≈ 15% of total units</li>
            <li><strong>C</strong>: ≈ 5% of total units</li>
        </ul>
    </li>
    <li>Print category-wise totals (e.g., count of SKUs and units in A, B, C).</li>
</ul>

<h3>Assumptions / Constraints</h3>

<ol>
    <li>
        Use exactly two structures and two fixed-size arrays:
        <ul>
            <li><code>SKU SKUList[MAX_SKU];</code></li>
            <li><code>Order OrderList[MAX_ORD];</code></li>
        </ul>
    </li>
    <li>
        Suggested limits (fixed and documented):
        <ul>
            <li><code>MAX_SKU = 3000</code></li>
            <li><code>MAX_ORD = 5000</code></li>
            <li><code>MAX_ITEMS = 20</code> (items per order)</li>
        </ul>
    </li>
    <li>
        IDs, time, and sorting preconditions:
        <ul>
            <li><code>skuId</code> and <code>orderId</code> must be unique and positive.</li>
            <li>Store time as integer in format <code>YYYYMMDDHHMM</code>.</li>
            <li>Binary search is allowed only when the array (or index view) is already sorted by the same key.</li>
        </ul>
    </li>
</ol>