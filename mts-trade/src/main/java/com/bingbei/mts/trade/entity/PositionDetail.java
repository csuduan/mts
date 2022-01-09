package com.bingbei.mts.trade.entity;

import com.bingbei.mts.common.entity.*;
import lombok.Data;

import java.io.Serializable;
import java.util.HashMap;


/**
 * @author sun0x00@gmail.com
 */
@Data
public class PositionDetail implements Serializable {

	private static final long serialVersionUID = 6210968936139034645L;

	private String symbol;
	private String tradingDay;
	private String strategyName;
	private String strategyID;

	private Integer contractSize = 0;
	private String exchange;

	private Double pnl = 0d;
	private Double openContractValue = 0d;

	private Integer longPos = 0;
	private Double longPnl = 0d;
	private Double longOpenContractValue = 0d;
	private Double longPrice = 0d;

	private Integer longYd = 0;
	private Integer longTd = 0;
	private Integer longPosFrozen = 0;
	private Integer longYdFrozen = 0;
	private Integer longTdFrozen = 0;
	private Integer longOpenFrozen = 0;

	private Integer shortPos = 0;
	private double shortPnl = 0;
	private double shortOpenContractValue = 0;
	private double shortPrice = 0;
	private Integer shortYd = 0;
	private Integer shortTd = 0;
	private Integer shortPosFrozen = 0;
	private Integer shortYdFrozen = 0;
	private Integer shortTdFrozen = 0;
	private Integer shortOpenFrozen = 0;

	private double lastPrice;

	private HashMap<String, Order> workingOrderMap = new HashMap<>();

	public PositionDetail(String rtSymbol, String rtAccountID, String tradingDay, String strategyName, String strategyID,
			String exchange, int contractSize) {
		this.symbol = rtSymbol;
		this.tradingDay = tradingDay;
		this.strategyName = strategyName;
		this.strategyID = strategyID;
		this.exchange = exchange;
		this.contractSize = contractSize;
	}

	public PositionDetail() {
	}



	public HashMap<String, Order> getWorkingOrderMap() {
		return workingOrderMap;
	}

	public void setWorkingOrderMap(HashMap<String, Order> workingOrderMap) {
		this.workingOrderMap = workingOrderMap;
	}

	/**
	 * 成交更新
	 * 
	 * @param trade
	 */
	public void updateTrade(Trade trade) {

		if (RtConstant.DIRECTION_LONG.equals(trade.getDirection())) {// 多头
			if (RtConstant.OFFSET_OPEN.equals(trade.getOffset())) {// 开仓
				longTd += trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSETODAY.equals(trade.getOffset())) {// 平今
				shortTd -= trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSEYESTERDAY.equals(trade.getOffset())) {// 平昨
				shortYd -= trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSE.equals(trade.getOffset())) {// 平仓
				if (RtConstant.EXCHANGE_SHFE.equals(exchange)) {// 上期所等同于平昨
					shortYd -= trade.getVolume();
				} else {
					// 非上期所,优先平今
					shortTd -= trade.getVolume();
					if (shortTd < 0) {
						shortYd += shortTd;
						shortTd = 0;
					}
				}
			}

		} else if (RtConstant.DIRECTION_SHORT.equals(trade.getDirection())) { // 空头
			// 开仓
			if (RtConstant.OFFSET_OPEN.equals(trade.getOffset())) {
				shortTd += trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSETODAY.equals(trade.getOffset())) {// 平今
				longTd -= trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSEYESTERDAY.equals(trade.getOffset())) {// 平昨
				longYd -= trade.getVolume();
			} else if (RtConstant.OFFSET_CLOSE.equals(trade.getOffset())) {// 平仓
				if (RtConstant.EXCHANGE_SHFE.equals(exchange)) {// 上期所等同于平昨
					longYd -= trade.getVolume();
				} else {// 非上期所,优先平今
					longTd -= trade.getVolume();
					if (longTd < 0) {
						longYd += longTd;
						longTd = 0;
					}
				}
			}
		}
		// 汇总今昨
		calculatePrice(trade);
		calculatePosition();
		calculatePnl();
	}

	/**
	 * 委托更新
	 * 
	 * @param order
	 */
	public void updateOrder(Order order) {
		// 将活动委托缓存下来
		if (RtConstant.STATUS_WORKING.contains(order.getStatus())) {
			workingOrderMap.put(order.getOrderRef(), order);

			// 移除缓存中已经完成的委托
		} else {
			if (workingOrderMap.containsKey(order.getOrderRef())) {
				workingOrderMap.remove(order.getOrderRef());
			}
		}

		// 计算冻结
		calculateFrozen();
	}

	/**
	 * 发单更新
	 * 
	 * @param orderReq
	 */
	public void updateOrderReq(OrderReq orderReq) {
		// orderReq如果存在则跳过
		if (workingOrderMap.containsKey(orderReq.getOriginalOrderID())) {
			return;
		}
		// 基于请求创建委托对象
		Order order = new Order();
		order.setSymbol(orderReq.getSymbol());
		order.setExchange(orderReq.getExchange());
		order.setOffset(orderReq.getOffset());
		order.setDirection(orderReq.getDirection());
		order.setTotalVolume(orderReq.getVolume());
		order.setStatus(RtConstant.STATUS_UNKNOWN);
		order.setAccountID(orderReq.getAccountID());

		//workingOrderMap.put(order.getOriginalOrderID(), order);

		calculateFrozen();

	}

	/**
	 * 价格更新
	 * 
	 * @param lastPrice
	 */
	public void updateLastPrice(double lastPrice) {
		this.lastPrice = lastPrice;
		calculatePnl();
	}

	/**
	 * 计算持仓盈亏
	 */
	public void calculatePnl() {
		longPnl = longPos * (lastPrice - longPrice) * contractSize;
		shortPnl = shortPos * (shortPrice - lastPrice) * contractSize;
		pnl = longPnl + shortPnl;

		longOpenContractValue = longPos * contractSize * longPrice;
		shortOpenContractValue = shortPos * contractSize * shortPrice;
		openContractValue = longOpenContractValue + shortOpenContractValue;
	}

	/**
	 * 计算持仓均价（基于成交数据）
	 * 
	 * @param trade
	 */
	public void calculatePrice(Trade trade) {
		// 只有开仓会影响持仓均价
		if (RtConstant.OFFSET_OPEN.equals(trade.getOffset())) {
			double cost = 0;
			int newPos = 0;
			if (RtConstant.DIRECTION_LONG.equals(trade.getDirection())) {
				cost = longPrice * longPos;
				cost += trade.getVolume() * trade.getPrice();
				newPos = longPos + trade.getVolume();
				if (newPos > 0) {
					longPrice = cost / newPos;
				} else {
					longPrice = 0d;
				}
			} else {
				cost = shortPrice * shortPos;
				cost += trade.getVolume() * trade.getPrice();
				newPos = shortPos + trade.getVolume();
				if (newPos > 0) {
					shortPrice = cost / newPos;
				} else {
					shortPrice = 0;
				}
			}
		}

	}

	public void calculatePosition() {
		longPos = longTd + longYd;
		shortPos = shortTd + shortYd;
	}

	/**
	 * 计算冻结
	 */
	public void calculateFrozen() {
		// 清空冻结数据
		longPosFrozen = 0;
		longYdFrozen = 0;
		longTdFrozen = 0;
		shortPosFrozen = 0;
		shortYdFrozen = 0;
		shortTdFrozen = 0;
		shortOpenFrozen = 0;
		longOpenFrozen = 0;

		int frozenVolume = 0;

		// 遍历统计
		for (Order order : workingOrderMap.values()) {
			// 计算剩余冻结量
			frozenVolume = order.getTotalVolume() - order.getTradedVolume();
			if (RtConstant.DIRECTION_LONG.equals(order.getDirection())) {// 多头委托
				if (RtConstant.OFFSET_CLOSETODAY.equals(order.getOffset())) {// 平今
					shortTdFrozen += frozenVolume;
				} else if (RtConstant.OFFSET_CLOSEYESTERDAY.equals(order.getOffset())) {// 平昨
					shortYdFrozen += frozenVolume;
				} else if (RtConstant.OFFSET_CLOSE.equals(order.getOffset())) {// 平仓
					shortTdFrozen += frozenVolume;
					if (shortTdFrozen > shortTd) {
						shortYdFrozen += (shortTdFrozen - shortTd);
						shortTdFrozen = shortTd;
					}
				} else if (RtConstant.OFFSET_OPEN.equals(order.getOffset())) {// 开仓
					if (RtConstant.STATUS_WORKING.contains(order.getStatus())) {
						longOpenFrozen += frozenVolume;
					}
				}
			} else if (RtConstant.DIRECTION_SHORT.equals(order.getDirection())) {// 空头委托
				if (RtConstant.OFFSET_CLOSETODAY.equals(order.getOffset())) { // 平今
					longTdFrozen += frozenVolume;
				} else if (RtConstant.OFFSET_CLOSEYESTERDAY.equals(order.getOffset())) { // 平昨
					longYdFrozen += frozenVolume;
				} else if (RtConstant.OFFSET_CLOSE.equals(order.getOffset())) {// 平仓
					longTdFrozen += frozenVolume;
					if (longTdFrozen > longTd) {
						longYdFrozen += (longTdFrozen - longTd);
						longTdFrozen = longTd;
					}
				} else if (RtConstant.OFFSET_OPEN.equals(order.getOffset())) {// 开仓
					if (RtConstant.STATUS_WORKING.contains(order.getStatus())) {
						shortOpenFrozen += frozenVolume;
					}
				}
			}
			// 汇总今昨冻结
			longPosFrozen = longYdFrozen + longTdFrozen;
			shortPosFrozen = shortYdFrozen + shortTdFrozen;
		}

	}


}